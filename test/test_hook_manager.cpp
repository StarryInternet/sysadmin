#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <log4cxx/logger.h>

#include "ConfigTypes.h"
#include "IExternalRunner.h"
#include "HookTable.h"
#include "TemplaterRunner.h"
#include "decibel/messaging/Reactor.h"

#include "HookManager.h"
#include "HookUser.h"

namespace sp = std::placeholders;

#define REQUIRE_EQUAL_VECTORS(a, b) EXPECT_THAT((a), ::testing::ContainerEq(b));

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_hook_manager"));
}

namespace dm = decibel::messaging;

namespace testhookmanager
{

class MockExternalService : public IExternalRunner
{
public:
    MockExternalService(std::shared_ptr<std::vector<size_t>> history, size_t runlevel)
      : mHistory(history)
      , mRunCalled(0)
      , mRunLevel(runlevel)
    {
        mRunCalled = 0;
    }

    folly::Future<folly::Unit> Run(const IExternalRunner::FormattedValues& ) const
    {
        mRunCalled++;
        mHistory->push_back(mRunLevel);
        return folly::makeFuture();
    }

    mutable std::shared_ptr<std::vector<size_t>> mHistory;
    mutable int mRunCalled;
    size_t mRunLevel;
};

}

struct HookManagerFixture : public ::testing::Test
{
    HookManagerFixture()
      : ::testing::Test()
      , mpTable(new HookTable)
      , mStorage()
      , mServicesCalledList(new std::vector<size_t>)
      , mMockTemplater(new testhookmanager::MockExternalService(std::make_shared<std::vector<size_t>>(), 1))
      , mMockServiceRunner(new testhookmanager::MockExternalService(mServicesCalledList, 1))
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    std::vector<ConfigPair::Key> FulfillFromStorage(const HookUser::HookContainer&)
    {
        return std::vector<ConfigPair::Key>();
    }

    std::unique_ptr<HookTable> mpTable;
    std::map<std::string, ConfigPair> mStorage;
    std::shared_ptr<std::vector<size_t>> mServicesCalledList;
    std::shared_ptr<testhookmanager::MockExternalService> mMockTemplater;
    std::shared_ptr<testhookmanager::MockExternalService> mMockServiceRunner;
};

TEST_F(HookManagerFixture, Basic)
{
    mpTable->Insert(HookUser("template1",
                             HookType::TEMPLATER,
                             std::vector<HookConfig>({HookConfig("hook1"),
                                         HookConfig("hook2")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage,
                                       this, sp::_1),
                             mMockTemplater, 1));
    mpTable->Insert(HookUser("service1",
                             HookType::SERVICE,
                             std::vector<HookConfig>({HookConfig("hook2"),
                                         HookConfig("hook3")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage,
                                       this, sp::_1),
                             mMockServiceRunner, 1));
    HookManager manager(std::move(mpTable), 100);
    // This Should Trigger both the template and service runners once each
    manager.HandleCommit(ConfigPairList({ConfigPair("hook1", ConfigType<std::string>("value"))}));
    ASSERT_EQ(1, mMockTemplater->mRunCalled);
    ASSERT_EQ(0, mMockServiceRunner->mRunCalled);

    manager.HandleCommit(ConfigPairList({ConfigPair("hook3", ConfigType<std::string>("value"))}));
    ASSERT_EQ(1, mMockTemplater->mRunCalled);
    ASSERT_EQ(1, mMockServiceRunner->mRunCalled);

    manager.HandleCommit(ConfigPairList({ConfigPair("hook2", ConfigType<std::string>("value"))}));
    ASSERT_EQ(2, mMockTemplater->mRunCalled);
    ASSERT_EQ(2, mMockServiceRunner->mRunCalled);

    manager.HandleCommit(ConfigPairList({ConfigPair("hook1", ConfigType<std::string>("value")),
                                         ConfigPair("hook2", ConfigType<std::string>("value"))}));
    // the key here is that both of the templaters hooks are satisfied by the committed items,
    // but the external templater is only run once
    ASSERT_EQ(3, mMockTemplater->mRunCalled);
    ASSERT_EQ(3, mMockServiceRunner->mRunCalled);
}

TEST_F(HookManagerFixture, OrderedServices)
{
    mpTable->Insert(HookUser("template1",
                             HookType::TEMPLATER,
                             std::vector<HookConfig>({HookConfig("hook1"),
                                         HookConfig("hook2")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage,
                                       this, sp::_1),
                             mMockTemplater, 1));
    mpTable->Insert(HookUser("service1",
                             HookType::SERVICE,
                             std::vector<HookConfig>({HookConfig("hook2"),
                                         HookConfig("hook3")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage,
                                       this, sp::_1),
                             mMockServiceRunner, 1));
    mpTable->Insert(HookUser("service2",
                             HookType::SERVICE,
                             std::vector<HookConfig>({HookConfig("hook2")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage,
                                       this, sp::_1),
                             mMockServiceRunner, 1));
    auto mockServiceRunner2 = std::make_shared<testhookmanager::MockExternalService>(mServicesCalledList, 2);
    mpTable->Insert(HookUser("service3",
                             HookType::SERVICE,
                             std::vector<HookConfig>({HookConfig("hook2"),
                                         HookConfig("hook3")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage,
                                       this, sp::_1),
                             mockServiceRunner2, 2));
    auto mockServiceRunner3 = std::make_shared<testhookmanager::MockExternalService>(mServicesCalledList, 3);
    mpTable->Insert(HookUser("service4",
                             HookType::SERVICE,
                             std::vector<HookConfig>({HookConfig("hook2"),
                                         HookConfig("hook3")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage,
                                       this, sp::_1),
                             mockServiceRunner3, 3));
    HookManager manager(std::move(mpTable), 100);

    manager.HandleCommit(ConfigPairList({ConfigPair("hook2", ConfigType<std::string>("value"))}));
    REQUIRE_EQUAL_VECTORS(*mServicesCalledList.get(), std::vector<size_t>({1, 1, 2, 3}));
}


TEST_F(HookManagerFixture, RealService)
{
    dm::Reactor r;
    std::vector<std::string> args = {"", ""};
    auto templater = std::make_shared<TemplaterRunner>(&r, "./argdumper.py", "failme", "dest", args);
    mpTable->Insert(HookUser("template1",
                             HookType::TEMPLATER,
                             std::vector<HookConfig>({HookConfig("hook1")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage, this, sp::_1),
                             templater, 1));
    HookManager manager(std::move(mpTable), 100);
    auto committed = ConfigPairList({ConfigPair("hook1", ConfigType<std::string>("value"))});
    int calledback = 0;
    manager.HandleCommit(committed).then([]()
    {
        FAIL() << "Should not have succeeded";
    }).onError([&calledback](const ExternalRunnerError&)
    {
        calledback++;
    });
    r.Start();
    ASSERT_EQ(1, calledback);
}

TEST_F(HookManagerFixture, DumpHooks)
{
    mpTable->Insert(HookUser("template1",
                             HookType::TEMPLATER,
                             std::vector<HookConfig>({HookConfig("hook1"),
                                         HookConfig("hook2")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage,
                                       this, sp::_1),
                             mMockTemplater, 1));
    mpTable->Insert(HookUser("service1",
                             HookType::SERVICE,
                             std::vector<HookConfig>({HookConfig("hook2"),
                                         HookConfig("hook3")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage,
                                       this, sp::_1),
                             mMockServiceRunner, 1));
    HookManager manager(std::move(mpTable), 100);

    HookManager::HookDump dump = manager.DumpHooks();
    EXPECT_EQ("template1", dump.first.at(0)->Name());
    EXPECT_EQ("service1", dump.second.at(0)->Name());
}

TEST_F(HookManagerFixture, TriggerHook)
{
    mpTable->Insert(HookUser("template1",
                             HookType::TEMPLATER,
                             std::vector<HookConfig>({HookConfig("hook1"),
                                         HookConfig("hook2")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage,
                                       this, sp::_1),
                             mMockTemplater, 1));
    mpTable->Insert(HookUser("service1",
                             HookType::SERVICE,
                             std::vector<HookConfig>({HookConfig("hook2"),
                                         HookConfig("hook3")}),
                             std::bind(&HookManagerFixture::FulfillFromStorage,
                                       this, sp::_1),
                             mMockServiceRunner, 1));
    HookManager manager(std::move(mpTable), 100);

    ASSERT_TRUE(manager.TriggerHook("template1").value());
    ASSERT_TRUE(manager.TriggerHook("service1").value());
    ASSERT_FALSE(manager.TriggerHook("nonexist").value());
}
