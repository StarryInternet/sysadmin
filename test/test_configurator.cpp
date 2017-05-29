#include <log4cxx/logger.h>
#include <gtest/gtest.h>

#include "ConfigTypes.h"
#include "Configurator.h"
#include "WildcardConfigurator.h"
#include "ExternalRunner.h"

#include "Helpers.h"

#include <map>

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_configurator"));
}

class TestConfiguratorFixture : public ::testing::Test
{
public:
    TestConfiguratorFixture()
      : ::testing::Test()
      , mBackingStore()
      , mStorage(&mBackingStore)
      , mTransactions()
    {

    }

    MockSystemStorage mBackingStore;
    WildcardConfigurator mStorage;
    MockTransactionStorage mTransactions;
};

TEST_F(TestConfiguratorFixture, BasicInteraction)
{
    Configurator conf(&mStorage, &mTransactions);

    conf.Set(ConfigPair("ssid", ConfigType<std::string>("cheesywaffle")), 0);
    ASSERT_FALSE(mBackingStore.ValueExists("ssid"));
    conf.Commit(0);
    ASSERT_TRUE(mBackingStore.ValueExists("ssid"));
}

TEST_F(TestConfiguratorFixture, MoreInteraction)
{
    Configurator conf(&mStorage, &mTransactions);

    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0);
    conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0);
    conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0);
    conf.Commit(0);

    conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(8)), 0);
    ASSERT_TRUE(mBackingStore.ValueExists("wireless.waffle"));
    ASSERT_TRUE(mBackingStore.ValueExists("wireless.cheese"));
    ASSERT_TRUE(mBackingStore.ValueExists("wireless.dynamite.explode"));
    ASSERT_EQ(5, UnwrapSinglePair(conf.Get(ConfigPair::Key("wireless.dynamite.explode"))).get().UnpackValue<int32_t>());

    conf.Commit(0);
    ASSERT_EQ("cheesywaffle", UnwrapSinglePair(conf.Get(ConfigPair::Key("wireless.waffle"))).get().UnpackValue<std::string>());
    ASSERT_EQ(true, UnwrapSinglePair(conf.Get(ConfigPair::Key("wireless.cheese"))).get().UnpackValue<bool>());
    ASSERT_EQ(8, UnwrapSinglePair(conf.Get(ConfigPair::Key("wireless.dynamite.explode"))).get().UnpackValue<int32_t>());

    ASSERT_FALSE(UnwrapSinglePair(conf.Get(ConfigPair::Key("this.is.not.set"))));
}

TEST_F(TestConfiguratorFixture, TypeCheckedInteraction)
{
    Configurator conf(&mStorage, &mTransactions);

    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0));
    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0));
    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0));
    conf.Commit(0);

    ASSERT_SET_SUCCESS(conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle2")), 0));
    ASSERT_SET_SUCCESS(conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(false)), 0));
    ASSERT_SET_SUCCESS(conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(25)), 0));
    conf.Commit(0);

    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.ssid", ConfigType<int32_t>(5)), 0));
    // I'm allowed the reset the type because it hasn't been set in system mStorage yet
    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.ssid", ConfigType<std::string>("true")), 0));
    conf.Commit(0);
    ASSERT_SET_TYPE_MISMATCH(conf.Set(ConfigPair("wireless.ssid", ConfigType<bool>(true)), 0));
}

TEST_F(TestConfiguratorFixture, TestDrop)
{
    Configurator conf(&mStorage, &mTransactions);

    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0));
    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0));
    conf.Drop(0);
    conf.Commit(0);
    ASSERT_FALSE(mBackingStore.ValueExists("wireless.waffle"));
    ASSERT_FALSE(mBackingStore.ValueExists("wireless.cheese"));
}

TEST_F(TestConfiguratorFixture, TestCommitSignal)
{
    Configurator conf(&mStorage, &mTransactions);

    conf.RegisterCommitHook([](const ConfigPairList& committed, bool)
    {
        auto front = committed.begin();
        EXPECT_EQ(true, front->UnpackValue<bool>());
        front++;
        EXPECT_EQ("cheesywaffle", front->UnpackValue<std::string>());
        front++;
        EXPECT_EQ(2, committed.size());
        return folly::makeFuture();
    });

    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0));
    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0));
    ASSERT_EQ(1, conf.Commit(0).value());
}

TEST_F(TestConfiguratorFixture, TestLockedQueue)
{
    Configurator conf(&mStorage, &mTransactions);

    auto pPromise = std::make_shared<folly::Promise<folly::Unit>>();
    conf.RegisterCommitHook([pPromise](const ConfigPairList& , bool)
    {
        return pPromise->getFuture();
    });

    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0));
    // Don't value() the commit to keep the items in flight
    conf.Commit(0);

    ASSERT_SET_LOCKED(conf.Set(ConfigPair("sad", ConfigType<std::string>("won't be set")), 0));
}

TEST_F(TestConfiguratorFixture, TestFailedCommitSignal)
{
    Configurator conf(&mStorage, &mTransactions);

    conf.RegisterCommitHook([](const ConfigPairList& , bool) -> folly::Future<folly::Unit>
    {
        return ExternalRunnerError("Catch me");
    });

    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0));
    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0));
    try
    {
        conf.Commit(0).value();
    }
    catch (const CommitError& err)
    {
        ASSERT_EQ(1, err.mCommitId);
    }

    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.sadness", ConfigType<std::string>(":(")), 0));
    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.failure", ConfigType<bool>(true)), 0));
    try
    {
        conf.Commit(0).value();
    }
    catch (const CommitError& err)
    {
        ASSERT_EQ(2, err.mCommitId);
    }
}

TEST_F(TestConfiguratorFixture, Erase)
{
    Configurator conf(&mStorage, &mTransactions);

    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0));
    ASSERT_SET_SUCCESS_NEW_KEY(conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0));
    conf.Commit(0);

    conf.Erase(ConfigPair::Key("wireless.waffle"), 0);
    // Erases have to be committed
    ASSERT_TRUE(mBackingStore.ValueExists("wireless.waffle"));
    conf.Commit(0);
    ASSERT_FALSE(mBackingStore.ValueExists("wireless.waffle"));
}

TEST_F(TestConfiguratorFixture, GetAllKeys)
{
    Configurator conf(&mStorage, &mTransactions);

    conf.Set(ConfigPair("key.house", ConfigType<int32_t>(123)), 0);
    conf.Set(ConfigPair("key.house.downstairs", ConfigType<std::string>("hey there")), 0);
    conf.Set(ConfigPair("house.downstairs.tv", ConfigType<bool>(true)), 0);
    conf.Commit(0);

    auto keys1 = conf.GetAllKeys(ConfigPair::Key("key.*"));
    REQUIRE_UNORDERED_EQUALS(std::vector<ConfigPair::Key>({ConfigKey("key.house.downstairs"), ConfigKey("key.house")}),
                             keys1);

    auto keys2 = conf.GetAllKeys(ConfigPair::Key("*"));
    REQUIRE_UNORDERED_EQUALS(std::vector<ConfigPair::Key>({ConfigKey("key.house.downstairs"), ConfigKey("key.house"), ConfigKey("house.downstairs.tv")}),
                             keys2);

    auto keys4 = conf.GetAllKeys(ConfigPair::Key("key.*.downstairs"));
    REQUIRE_UNORDERED_EQUALS(std::vector<ConfigPair::Key>({ConfigKey("key.house.downstairs")}),
                             keys4);

    auto keys5 = conf.GetAllKeys(ConfigPair::Key("*.house.*"));
    REQUIRE_UNORDERED_EQUALS(std::vector<ConfigPair::Key>({ConfigKey("key.house.downstairs")}),
                             keys5);

    auto keys3 = conf.GetAllKeys(ConfigPair::Key("key.house.not.real.*"));
    ASSERT_EQ(0, keys3.size());
}

TEST_F(TestConfiguratorFixture, DifferentIds)
{
    Configurator conf(&mStorage, &mTransactions);

    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0);
    conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0);
    conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0);
    // Different ID, so the above aren't committed
    conf.Commit(1);

    ASSERT_FALSE(mBackingStore.ValueExists("wireless.waffle"));
    ASSERT_FALSE(mBackingStore.ValueExists("wireless.cheese"));
    ASSERT_FALSE(mBackingStore.ValueExists("wireless.dynamite.explode"));

    // Same ID now, so they should be committed
    conf.Commit(0);
    ASSERT_EQ("cheesywaffle", UnwrapSinglePair(conf.Get(ConfigPair::Key("wireless.waffle"))).get().UnpackValue<std::string>());
    ASSERT_EQ(true, UnwrapSinglePair(conf.Get(ConfigPair::Key("wireless.cheese"))).get().UnpackValue<bool>());
    ASSERT_EQ(5, UnwrapSinglePair(conf.Get(ConfigPair::Key("wireless.dynamite.explode"))).get().UnpackValue<int32_t>());

    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("steak")), 5);
    conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(false)), 5);
    conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(8)), 5);
    // Dropping the wrong ID doesn't drop our values
    conf.Drop(0);
    conf.Commit(5);

    ASSERT_EQ("steak", UnwrapSinglePair(conf.Get(ConfigPair::Key("wireless.waffle"))).get().UnpackValue<std::string>());
    ASSERT_EQ(false, UnwrapSinglePair(conf.Get(ConfigPair::Key("wireless.cheese"))).get().UnpackValue<bool>());
    ASSERT_EQ(8, UnwrapSinglePair(conf.Get(ConfigPair::Key("wireless.dynamite.explode"))).get().UnpackValue<int32_t>());
}

TEST_F(TestConfiguratorFixture, MaxTransactions)
{
    Configurator conf(&mStorage, &mTransactions, 2);

    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0);
    conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0);
    conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0);

    conf.Set(ConfigPair("stuff", ConfigType<int32_t>(9)), 1);

    // This should kick out the older set of transactions with id 0
    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("too bad")), 2);

    // No affect since the transactions were dropped
    conf.Commit(0);
    ASSERT_FALSE(mBackingStore.ValueExists("wireless.waffle"));
    ASSERT_FALSE(mBackingStore.ValueExists("wireless.cheese"));
    ASSERT_FALSE(mBackingStore.ValueExists("wireless.dynamite.explode"));

    conf.Commit(1);
    ASSERT_EQ(9, UnwrapSinglePair(conf.Get(ConfigPair::Key("stuff"))).get().UnpackValue<int32_t>());

    // Should now allow more transactions since one was cleared
    conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(false)), 3);
    conf.Commit(3);
    ASSERT_EQ(false, UnwrapSinglePair(conf.Get(ConfigPair::Key("wireless.cheese"))).get().UnpackValue<bool>());
}

TEST_F(TestConfiguratorFixture, EraseNamespace)
{
    Configurator conf(&mStorage, &mTransactions, 2);

    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0);
    conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0);
    conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0);

    conf.Commit(0);

    conf.Erase(ConfigPair::Key("wireless.*"), 0);
    conf.Commit(0);

    ASSERT_FALSE(mBackingStore.ValueExists("wireless.waffle"));
    ASSERT_FALSE(mBackingStore.ValueExists("wireless.cheese"));
    ASSERT_FALSE(mBackingStore.ValueExists("wireless.dynamite.explode"));

    conf.Set(ConfigPair("wireless", ConfigType<std::string>("dumb")), 0);
    conf.Erase(ConfigPair::Key("wireless.*"), 0);
    conf.Commit(0);

    ASSERT_TRUE(mBackingStore.ValueExists("wireless"));
}

TEST_F(TestConfiguratorFixture, LastItemsNamespace)
{
    Configurator conf(&mStorage, &mTransactions, 2);

    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0);
    conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0);
    conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0);

    conf.Commit(0);
    ASSERT_FALSE(mBackingStore.ValueExists("last.wireless.waffle"));

    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("no more cheese")), 0);
    conf.Commit(0);

    // The old value for wireless.waffle is stored in the last.* namespace
    ASSERT_EQ("cheesywaffle",
              UnwrapSinglePair(conf.Get(ConfigPair::Key("last.wireless.waffle"))).get().UnpackValue<std::string>());

    conf.Erase(ConfigPair::Key("wireless.waffle"), 0);
    conf.Commit(0);
    ASSERT_EQ("no more cheese",
              UnwrapSinglePair(conf.Get(ConfigPair::Key("last.wireless.waffle"))).get().UnpackValue<std::string>());

    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheeseisback")), 0);
    conf.Commit(0);
    ASSERT_FALSE(mBackingStore.ValueExists("last.wireless.waffle"));
}

TEST_F(TestConfiguratorFixture, EmptyCommits)
{
    Configurator conf(&mStorage, &mTransactions, 2);

    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0);
    conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0);
    conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0);

    conf.Commit(0);
    ASSERT_EQ(1, mTransactions.GetCommitCount());

    // Setting and committing the same value as existing should not increase commit count
    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0);
    conf.Commit(0);
    ASSERT_EQ(1, mTransactions.GetCommitCount());
}

TEST_F(TestConfiguratorFixture, InFlightItems)
{
    Configurator conf(&mStorage, &mTransactions, 2);

    auto items = conf.InFlightItems(5);
    ASSERT_EQ(0, items.size());

    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0);
    conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0);
    conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0);

    auto items1 = conf.InFlightItems(0);
    ASSERT_EQ(3, items1.size());

    conf.Commit(0);
    auto items2 = conf.InFlightItems(0);
    ASSERT_EQ(0, items2.size());
}

TEST_F(TestConfiguratorFixture, FollyFormatting)
{
    Configurator conf(&mStorage, &mTransactions, 2);

    conf.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0);
    conf.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0);
    conf.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0);
    conf.Commit(0);

    ASSERT_EQ("cheesywaffle", folly::svformat("{wireless.waffle}", (IConfigurator*)(&conf)));
    ASSERT_EQ("true", folly::svformat("{wireless.cheese}", (IConfigurator*)(&conf)));
    ASSERT_EQ("5", folly::svformat("{wireless.dynamite.explode}", (IConfigurator*)(&conf)));
    ASSERT_EQ("", folly::svformat("{notreal}", (IConfigurator*)(&conf)));
}
