#include <log4cxx/logger.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ExternalRunner.h"
#include "SysAdminController.h"
#include "Configurator.h"
#include "ConfigTypes.h"
#include "HookManager.h"
#include "HookTable.h"
#include "sysadmin-api/sysadminctl.pb.h"

#include "Helpers.h"
#include <random>

using namespace std::placeholders;

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_protobuf_translator"));
}

namespace testsysadmin
{

class MockExternalService : public IExternalRunner
{
public:
    MockExternalService()
      : mRunCalled(0)
    {
        mRunCalled = 0;
    }

    folly::Future<folly::Unit> Run(const IExternalRunner::FormattedValues& ) const
    {
        mRunCalled++;
        return folly::makeFuture();
    }

    mutable int mRunCalled;
};

class MockFailedService : public IExternalRunner
{
public:
    MockFailedService()
    {
    }

    folly::Future<folly::Unit> Run(const IExternalRunner::FormattedValues& ) const
    {
        throw ExternalRunnerError("Failure");
    }
};

}

struct SysAdminControllerFixture : public ::testing::Test
{
    SysAdminControllerFixture()
      : ::testing::Test()
      , mStorage()
      , mTransactions()
      , mpTable(new HookTable)
      , mMockTemplater(new testsysadmin::MockExternalService)
      , mMockFailedTemplater(new testsysadmin::MockFailedService)
      , mMockServiceRunner(new testsysadmin::MockExternalService)
    {
    }

    virtual void SetUp()
    {
        mpTable->Insert(HookUser("template1",
                                 HookType::TEMPLATER,
                                 std::vector<HookConfig>({HookConfig("hook1"),
                                             HookConfig("hook2")}),
                                 std::bind(&SysAdminControllerFixture::FulfillFromStorage,
                                           this, _1),
                                 mMockTemplater, 1));
        mpTable->Insert(HookUser("service1",
                                 HookType::SERVICE,
                                 std::vector<HookConfig>({HookConfig("hook2"),
                                             HookConfig("hook3")}),
                                 std::bind(&SysAdminControllerFixture::FulfillFromStorage,
                                           this, _1),
                                 mMockServiceRunner, 1));
    }

    virtual void TearDown()
    {
    }

    std::vector<ConfigPair::Key> FulfillFromStorage(const HookUser::HookContainer&)
    {
        return std::vector<ConfigPair::Key>();
    }

    MockSystemStorage mStorage;
    MockTransactionStorage mTransactions;
    std::unique_ptr<HookTable> mpTable;
    std::shared_ptr<testsysadmin::MockExternalService> mMockTemplater;
    std::shared_ptr<testsysadmin::MockFailedService> mMockFailedTemplater;
    std::shared_ptr<testsysadmin::MockExternalService> mMockServiceRunner;
};

uint32_t RandomUint32()
{
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_int_distribution<uint32_t> intDistribution;

    return intDistribution(rng);
}

sysadminctl::Command GenerateSetCommand(std::string key, std::string value)
{
    sysadminctl::Command command;
    command.set_id(RandomUint32());
    auto confvalue = command.mutable_set()->mutable_value();
    confvalue->set_strval(value);
    command.mutable_set()->set_key(key);

    return command;
}

sysadminctl::Command GenerateGetCommand(std::string key)
{
    sysadminctl::Command command;
    command.set_id(RandomUint32());
    auto confvalue = command.mutable_get();
    confvalue->set_key(key);

    return command;
}

sysadminctl::Command GenerateEraseCommand(std::string key)
{
    sysadminctl::Command command;
    command.set_id(RandomUint32());
    auto confvalue = command.mutable_erase();
    confvalue->set_key(key);

    return command;
}

sysadminctl::Command GenerateCommitCommand(bool run_hooks = true)
{
    sysadminctl::Command command;
    command.set_id(RandomUint32());
    auto commit = command.mutable_commit();
    if (!run_hooks)
    {
        commit->set_config(sysadminctl::CommitConfig::NO_HOOKS);
    }

    return command;
}

sysadminctl::Command GenerateRollbackCommand(CommitHistory::CommitId id)
{
    sysadminctl::Command command;
    command.set_id(RandomUint32());
    command.mutable_rollback()->set_id(id);

    return command;
}

sysadminctl::Command GenerateBlameCommand(std::string key)
{
    sysadminctl::Command command;
    command.set_id(RandomUint32());
    auto blame = command.mutable_blame();
    blame->set_key(key);
    return command;
}

TEST_F(SysAdminControllerFixture, BasicTest)
{
    Configurator config(&mStorage, &mTransactions);
    std::unique_ptr<HookTable> pTable(new HookTable);
    auto mgmr = std::make_shared<HookManager>(std::move(pTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    sysadminctl::Command cmd = GenerateSetCommand("waffle.house", "thevalue.com");
    auto goodresp = boost::get(controller.SetHandler(cmd, 0).value());
    ASSERT_EQ("waffle.house", goodresp.get().kvs(0).key());
    ASSERT_EQ("thevalue.com", goodresp.get().kvs(0).value().strval());
    ASSERT_FALSE(mStorage.ValueExists("waffle.house"));
    controller.CommitHandler(GenerateCommitCommand(), 0).value();
    ASSERT_TRUE(mStorage.ValueExists("waffle.house"));

    sysadminctl::Command getcmd = GenerateGetCommand("waffle.house");
    auto resp = boost::get(controller.GetHandler(getcmd, 0).value());
    ASSERT_EQ(resp.get().kvs(0).key(), "waffle.house");
    ASSERT_EQ(resp.get().kvs(0).value().strval(), "thevalue.com");
    ASSERT_EQ(getcmd.id(), resp.id());

    sysadminctl::Command failedget = GenerateGetCommand("cheese.house");
    auto failedresp = boost::get(controller.GetHandler(failedget, 0).value());
    ASSERT_EQ(sysadminctl::StatusCode::KEY_NOT_FOUND, failedresp.status());
    ASSERT_EQ(failedget.id(), failedresp.id());

    sysadminctl::Command badtypeset = GenerateSetCommand("bad.ssid",
                                                         "ssid is actually a number here");
    // We're manually setting via the configurator to force bad.ssid to be an int
    config.Set(ConfigPair("bad.ssid", ConfigType<int32_t>(4)), 0);
    config.Commit(0);
    auto badtyperesp = boost::get(controller.SetHandler(badtypeset, 0).value());
    ASSERT_EQ(sysadminctl::StatusCode::TYPE_MISMATCH, badtyperesp.status());
    ASSERT_EQ(badtypeset.id(), badtyperesp.id());

    // We don't allow setting keys that contain "*"s, as taht's the wildcard character
    sysadminctl::Command badsetcmd = GenerateSetCommand("waffle.*.house", "value");
    auto badsetresp = boost::get(controller.SetHandler(badsetcmd, 0).value());
    ASSERT_EQ(sysadminctl::StatusCode::INVALID_KEY, badsetresp.status());

    // Users cannot set keys in the last.* namespace
    sysadminctl::Command lastsetcmd = GenerateSetCommand("last.cheese.waffle", "value");
    auto lastsetresp = boost::get(controller.SetHandler(lastsetcmd, 0).value());
    ASSERT_EQ(sysadminctl::StatusCode::INVALID_KEY, lastsetresp.status());

    // Test return status code for new key creation
    sysadminctl::Command newsetcmd = GenerateSetCommand("waffle.icecream", "value");
    auto newkeyresp = boost::get(controller.SetHandler(newsetcmd, 0).value());
    ASSERT_EQ(sysadminctl::StatusCode::SUCCESS_KEY_CREATED, newkeyresp.status());
}

TEST_F(SysAdminControllerFixture, DropTest)
{
    Configurator config(&mStorage, &mTransactions);
    std::unique_ptr<HookTable> pTable(new HookTable);
    auto mgmr = std::make_shared<HookManager>(std::move(pTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    sysadminctl::Command cmd = GenerateSetCommand("waffle.house", "thevalue.com");
    controller.SetHandler(cmd, 0).value();
    ASSERT_FALSE(mStorage.ValueExists("waffle.house"));
    // We ignore the commands in both of these calls so I reuse the Commit Command
    controller.DropHandler(GenerateCommitCommand(), 0).value();
    controller.CommitHandler(GenerateCommitCommand(), 0).value();
    ASSERT_FALSE(mStorage.ValueExists("waffle.house"));
}

TEST_F(SysAdminControllerFixture, FireHooksTest)
{
    Configurator config(&mStorage, &mTransactions);
    auto mgmr = std::make_shared<HookManager>(std::move(mpTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    // Seeding mStorage with some values so the hooks run
    mStorage.Set(ConfigPair("hook1", ConfigType<std::string>("result")), 0);
    mStorage.Set(ConfigPair("notahook", ConfigType<std::string>("result")), 0);
    mStorage.Commit(0);
    controller.FireHooksHandler(GenerateCommitCommand(), 0).value();
    ASSERT_EQ(1, mMockTemplater->mRunCalled);
    ASSERT_EQ(1, mMockServiceRunner->mRunCalled);

    mStorage.Set(ConfigPair("hook2", ConfigType<std::string>("result")), 0);
    mStorage.Set(ConfigPair("hook3", ConfigType<std::string>("result")), 0);
    mStorage.Commit(0);
    controller.FireHooksHandler(GenerateCommitCommand(), 0).value();
    ASSERT_EQ(2, mMockTemplater->mRunCalled);
    ASSERT_EQ(2, mMockServiceRunner->mRunCalled);
}

TEST_F(SysAdminControllerFixture, EraseTest)
{
    Configurator config(&mStorage, &mTransactions);
    std::unique_ptr<HookTable> pTable(new HookTable);
    auto mgmr = std::make_shared<HookManager>(std::move(pTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    sysadminctl::Command cmd = GenerateSetCommand("waffle.house", "thevalue.com");
    auto goodresp = boost::get(controller.SetHandler(cmd, 0).value());
    controller.CommitHandler(GenerateCommitCommand(), 0).value();
    ASSERT_TRUE(mStorage.ValueExists("waffle.house"));

    sysadminctl::Command cmd2 = GenerateEraseCommand("waffle.house");
    controller.EraseHandler(cmd2, 0).value();
    ASSERT_TRUE(mStorage.ValueExists("waffle.house"));
    controller.CommitHandler(GenerateCommitCommand(), 0).value();
    ASSERT_FALSE(mStorage.ValueExists("waffle.house"));
}

TEST_F(SysAdminControllerFixture, GetAllKeysTest)
{
    Configurator config(&mStorage, &mTransactions);
    std::unique_ptr<HookTable> pTable(new HookTable);
    auto mgmr = std::make_shared<HookManager>(std::move(pTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    mStorage.Set(ConfigPair("wildcard.stuff", ConfigType<std::string>("result")), 0);
    mStorage.Set(ConfigPair("wildcard.other.stuff", ConfigType<std::string>("result")), 0);
    mStorage.Set(ConfigPair("no.wildcard.stuff", ConfigType<std::string>("result")), 0);
    mStorage.Commit(0);

    auto goodcmd = GenerateGetCommand("wildcard.*");
    auto goodresp = boost::get(controller.GetHandler(goodcmd, 0).value());
    ASSERT_EQ(2, goodresp.get().kvs_size());
}

TEST_F(SysAdminControllerFixture, OverrideClientId)
{
    Configurator config(&mStorage, &mTransactions);
    std::unique_ptr<HookTable> pTable(new HookTable);
    auto mgmr = std::make_shared<HookManager>(std::move(pTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    sysadminctl::Command cmd = GenerateSetCommand("waffle.house", "thevalue.com");
    cmd.set_xid(1);
    auto goodresp = boost::get(controller.SetHandler(cmd, 0).value());
    ASSERT_EQ("waffle.house", goodresp.get().kvs(0).key());
    ASSERT_EQ("thevalue.com", goodresp.get().kvs(0).value().strval());
    ASSERT_FALSE(mStorage.ValueExists("waffle.house"));
    controller.CommitHandler(GenerateCommitCommand(), 0).value();
    // The above doesn't commit our value because we overrode the clientId
    ASSERT_FALSE(mStorage.ValueExists("waffle.house"));
    controller.CommitHandler(GenerateCommitCommand(), 1).value();
    // Now it's actually there
    ASSERT_TRUE(mStorage.ValueExists("waffle.house"));

    sysadminctl::Command cmd2 = GenerateSetCommand("waffle.cheese", "cheddar");
    cmd2.set_xid(1);
    controller.SetHandler(cmd2, 0).value();
    // We drop 0, which has no affect on our overridden set
    controller.DropHandler(GenerateCommitCommand(), 0).value();
    sysadminctl::Command cmd3 = GenerateCommitCommand();
    cmd3.set_xid(1);
    // Now we commit it
    controller.CommitHandler(cmd3, 0).value();
    ASSERT_TRUE(mStorage.ValueExists("waffle.cheese"));

    sysadminctl::Command cmd4 = GenerateEraseCommand("waffle.cheese");
    cmd4.set_xid(1);
    controller.EraseHandler(cmd4, 0).value();
    controller.CommitHandler(GenerateCommitCommand(), 1).value();
    ASSERT_FALSE(mStorage.ValueExists("waffle.cheese"));
}

TEST_F(SysAdminControllerFixture, FailingHookOnCommit)
{
    std::unique_ptr<HookTable> pTable(new HookTable);
    pTable->Insert(HookUser("template1",
                            HookType::TEMPLATER,
                            std::vector<HookConfig>({HookConfig("hook1")}),
                            std::bind(&SysAdminControllerFixture::FulfillFromStorage, this, _1),
                            mMockFailedTemplater, 1));

    Configurator config(&mStorage, &mTransactions);
    auto mgmr = std::make_shared<HookManager>(std::move(pTable), 100);
    SysAdminController controller(&config, &config, mgmr);
    config.RegisterCommitHook(std::bind(&HookManager::HandleCommit, mgmr, _1, _2));

    sysadminctl::Command cmd = GenerateSetCommand("hook1", "value");
    boost::get(controller.SetHandler(cmd, 0).value());
    auto failedresp = boost::get(controller.CommitHandler(GenerateCommitCommand(), 0).value());
    ASSERT_EQ(sysadminctl::StatusCode::EXTERNAL_PROCESS_ERROR, failedresp.status());
    ASSERT_TRUE(failedresp.commit().has_commit_id());
    ASSERT_EQ(1, failedresp.commit().commit_id());

    cmd = GenerateSetCommand("hook1", "othervalue");
    boost::get(controller.SetHandler(cmd, 0).value());
    failedresp = boost::get(controller.CommitHandler(GenerateCommitCommand(), 0).value());
    ASSERT_EQ(sysadminctl::StatusCode::EXTERNAL_PROCESS_ERROR, failedresp.status());
    ASSERT_TRUE(failedresp.commit().has_commit_id());
    ASSERT_EQ(2, failedresp.commit().commit_id());
}

TEST_F(SysAdminControllerFixture, Rollback)
{
    Configurator config(&mStorage, &mTransactions);
    std::unique_ptr<HookTable> pTable(new HookTable);
    auto mgmr = std::make_shared<HookManager>(std::move(pTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    sysadminctl::Command set = GenerateSetCommand("key", "value");
    boost::get(controller.SetHandler(set, 0).value());
    boost::get(controller.CommitHandler(GenerateCommitCommand(), 0).value());

    sysadminctl::Command newset = GenerateSetCommand("key", "newvalue");
    boost::get(controller.SetHandler(newset, 0).value());
    boost::get(controller.CommitHandler(GenerateCommitCommand(), 0).value());

    // Since nothing else is in the system the last commit should be id 2
    sysadminctl::Command rollback = GenerateRollbackCommand(2);
    boost::get(controller.RollbackHandler(rollback, 0).value());

    sysadminctl::Command getcmd = GenerateGetCommand("key");
    auto resp = boost::get(controller.GetHandler(getcmd, 0).value());
    ASSERT_EQ("value", resp.get().kvs(0).value().strval());

    sysadminctl::Command badrollback = GenerateRollbackCommand(100);
    auto badroll = boost::get(controller.RollbackHandler(badrollback, 0).value());
    ASSERT_EQ(sysadminctl::StatusCode::FAILED_ROLLBACK, badroll.status());
}

TEST_F(SysAdminControllerFixture, SkippingHooks)
{
    std::unique_ptr<HookTable> pTable(new HookTable);
    pTable->Insert(HookUser("template1",
                            HookType::TEMPLATER,
                            std::vector<HookConfig>({HookConfig("hook1")}),
                            std::bind(&SysAdminControllerFixture::FulfillFromStorage, this, _1),
                            mMockFailedTemplater, 1));

    Configurator config(&mStorage, &mTransactions);
    auto mgmr = std::make_shared<HookManager>(std::move(pTable), 100);
    SysAdminController controller(&config, &config, mgmr);
    config.RegisterCommitHook(std::bind(&HookManager::HandleCommit, mgmr, _1, _2));

    sysadminctl::Command cmd = GenerateSetCommand("hook1", "value");
    boost::get(controller.SetHandler(cmd, 0).value());
    auto failedresp = boost::get(controller.CommitHandler(GenerateCommitCommand(false), 0).value());
    ASSERT_EQ(sysadminctl::StatusCode::SUCCESS, failedresp.status());
}

TEST_F(SysAdminControllerFixture, Reset)
{
    Configurator config(&mStorage, &mTransactions);
    std::unique_ptr<HookTable> pTable(new HookTable);
    auto mgmr = std::make_shared<HookManager>(std::move(pTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    boost::get(controller.SetHandler(GenerateSetCommand("key1", "notdefault"), 0).value());
    boost::get(controller.SetHandler(GenerateSetCommand("default.key1", "value1"), 0).value());
    boost::get(controller.SetHandler(GenerateSetCommand("key2", "value2"), 0).value());
    boost::get(controller.SetHandler(GenerateSetCommand("default.key2", "value2"), 0).value());
    boost::get(controller.SetHandler(GenerateSetCommand("key3", "otherthing"), 0).value());
    boost::get(controller.SetHandler(GenerateSetCommand("default.key3", "value3"), 0).value());
    boost::get(controller.SetHandler(GenerateSetCommand("notdefault", ":("), 0).value());
    boost::get(controller.CommitHandler(GenerateCommitCommand(), 0).value());

    ASSERT_EQ(sysadminctl::StatusCode::SUCCESS,
              boost::get(controller.ResetHandler(GenerateCommitCommand(), 0).value()).status());

    auto resp1 = boost::get(controller.GetHandler(GenerateGetCommand("key1"), 0).value());
    ASSERT_EQ("value1", resp1.get().kvs(0).value().strval());

    auto resp2 = boost::get(controller.GetHandler(GenerateGetCommand("key2"), 0).value());
    ASSERT_EQ("value2", resp2.get().kvs(0).value().strval());

    auto resp3 = boost::get(controller.GetHandler(GenerateGetCommand("key3"), 0).value());
    ASSERT_EQ("value3", resp3.get().kvs(0).value().strval());

    auto resp4 = boost::get(controller.GetHandler(GenerateGetCommand("notdefault"), 0).value());
    ASSERT_EQ(sysadminctl::StatusCode::KEY_NOT_FOUND, resp4.status());

    auto resp5 = boost::get(controller.GetHandler(GenerateGetCommand("last.key1"), 0).value());
    ASSERT_EQ("notdefault", resp5.get().kvs(0).value().strval());

    auto resp6 = boost::get(controller.GetHandler(GenerateGetCommand("last.key2"), 0).value());
    ASSERT_EQ("value2", resp6.get().kvs(0).value().strval());

    auto resp7 = boost::get(controller.GetHandler(GenerateGetCommand("last.key3"), 0).value());
    ASSERT_EQ("otherthing", resp7.get().kvs(0).value().strval());
}

TEST_F(SysAdminControllerFixture, DumpHooks)
{
    Configurator config(&mStorage, &mTransactions);
    auto mgmr = std::make_shared<HookManager>(std::move(mpTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    sysadminctl::Command cmd;
    cmd.set_id(1);
    cmd.mutable_dump();

    auto resp = *(controller.DumpHooksHandler(cmd, 0).value());
    ASSERT_EQ(1, resp.id());
    const sysadminctl::DumpResponse& dumpResp = resp.dump();
    EXPECT_EQ("template1", dumpResp.templatehooks(0));
    EXPECT_EQ("service1", dumpResp.servicehooks(0));
}

TEST_F(SysAdminControllerFixture, TriggerHook)
{
    Configurator config(&mStorage, &mTransactions);
    auto mgmr = std::make_shared<HookManager>(std::move(mpTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    sysadminctl::Command goodCmd;
    goodCmd.set_id(1);
    sysadminctl::TriggerHook* triggerGoodCmd = goodCmd.mutable_trigger();
    triggerGoodCmd->set_hook("service1");

    auto goodResp = *(controller.TriggerHookHandler(goodCmd, 0).value());
    ASSERT_EQ(1, goodResp.id());
    EXPECT_EQ(sysadminctl::StatusCode::SUCCESS, goodResp.status());

    sysadminctl::Command badCmd;
    badCmd.set_id(2);
    sysadminctl::TriggerHook* triggerBadCmd = badCmd.mutable_trigger();
    triggerBadCmd->set_hook("doesntExist");

    auto badResp = *(controller.TriggerHookHandler(badCmd, 0).value());
    ASSERT_EQ(2, badResp.id());
    EXPECT_EQ(sysadminctl::StatusCode::HOOK_NOT_FOUND, badResp.status());
}

TEST_F(SysAdminControllerFixture, Blame)
{
    Configurator config(&mStorage, &mTransactions);
    std::unique_ptr<HookTable> pTable(new HookTable);
    auto mgmr = std::make_shared<HookManager>(std::move(pTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    auto resp1 = controller.BlameHandler(GenerateBlameCommand("key1"), 0).value();
    ASSERT_EQ(0, resp1.get().blame().entries_size());

    controller.SetHandler(GenerateSetCommand("key1", "val1"), 0);
    controller.SetHandler(GenerateSetCommand("key2", "val2"), 0);
    controller.CommitHandler(GenerateCommitCommand(), 0);

    auto resp2 = controller.BlameHandler(GenerateBlameCommand("key1"), 0).value();
    ASSERT_EQ(1, resp2.get().blame().entries_size());
    auto resp3 = controller.BlameHandler(GenerateBlameCommand("key2"), 0).value();
    ASSERT_EQ(1, resp3.get().blame().entries_size());

    controller.SetHandler(GenerateSetCommand("key1", "val2"), 0);
    controller.CommitHandler(GenerateCommitCommand(), 0);

    auto resp4 = controller.BlameHandler(GenerateBlameCommand("key1"), 0).value();
    ASSERT_EQ(2, resp4.get().blame().entries_size());
    ASSERT_EQ("val2", resp4.get().blame().entries(0).val().strval());
    ASSERT_EQ("val1", resp4.get().blame().entries(1).val().strval());

    auto resp5 = controller.BlameHandler(GenerateBlameCommand("key2"), 0).value();
    ASSERT_EQ(1, resp5.get().blame().entries_size());
    ASSERT_EQ("val2", resp5.get().blame().entries(0).val().strval());
}

TEST_F(SysAdminControllerFixture, InFlight)
{
    Configurator config(&mStorage, &mTransactions);
    std::unique_ptr<HookTable> pTable(new HookTable);
    auto mgmr = std::make_shared<HookManager>(std::move(pTable), 100);
    SysAdminController controller(&config, &config, mgmr);

    auto resp1 = controller.InFlightHandler(GenerateCommitCommand(), 0).value();
    ASSERT_EQ(0, resp1.get().inflight().kvs_size());

    controller.SetHandler(GenerateSetCommand("key1", "val1"), 0);
    controller.SetHandler(GenerateSetCommand("key2", "val2"), 0);

    auto resp2 = controller.InFlightHandler(GenerateCommitCommand(), 0).value();
    ASSERT_EQ(2, resp2.get().inflight().kvs_size());

    controller.CommitHandler(GenerateCommitCommand(), 0);
    auto resp3 = controller.InFlightHandler(GenerateCommitCommand(), 0).value();
    ASSERT_EQ(0, resp3.get().inflight().kvs_size());
}
