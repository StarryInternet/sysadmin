#include <log4cxx/logger.h>
#include <gtest/gtest.h>

#include "ConfigTypes.h"
#include "Configurator.h"
#include "PassThroughConfigurator.h"
#include "ExternalRunner.h"

#include "Helpers.h"

#include <map>

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_configurator"));
}

class TestPassThroughConfiguratorFixture : public ::testing::Test
{
public:
    TestPassThroughConfiguratorFixture()
      : ::testing::Test()
      , mStorage()
      , mTransactions()
    {

    }

    MockSystemStorage mStorage;
    MockTransactionStorage mTransactions;
};

TEST_F(TestPassThroughConfiguratorFixture, BasicInteraction)
{
    Configurator conf(&mStorage, &mTransactions);
    PassThroughConfigurator passer(&conf);

    passer.Set(ConfigPair("ssid", ConfigType<std::string>("cheesywaffle")), 0);
    ASSERT_FALSE(mStorage.ValueExists("ssid"));
    passer.Commit(0);
    ASSERT_TRUE(mStorage.ValueExists("ssid"));
}

TEST_F(TestPassThroughConfiguratorFixture, MoreInteraction)
{
    Configurator conf(&mStorage, &mTransactions);
    PassThroughConfigurator passer(&conf);

    passer.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0);
    passer.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0);
    passer.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0);
    passer.Commit(0);

    passer.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(8)), 0);
    ASSERT_TRUE(mStorage.ValueExists("wireless.waffle"));
    ASSERT_TRUE(mStorage.ValueExists("wireless.cheese"));
    ASSERT_TRUE(mStorage.ValueExists("wireless.dynamite.explode"));
    ASSERT_EQ(5, UnwrapSinglePair(passer.Get(ConfigPair::Key("wireless.dynamite.explode"))).get().UnpackValue<int32_t>());

    passer.Commit(0);
    ASSERT_EQ("cheesywaffle", UnwrapSinglePair(passer.Get(ConfigPair::Key("wireless.waffle"))).get().UnpackValue<std::string>());
    ASSERT_EQ(true, UnwrapSinglePair(passer.Get(ConfigPair::Key("wireless.cheese"))).get().UnpackValue<bool>());
    ASSERT_EQ(8, UnwrapSinglePair(passer.Get(ConfigPair::Key("wireless.dynamite.explode"))).get().UnpackValue<int32_t>());

    ASSERT_FALSE(UnwrapSinglePair(passer.Get(ConfigPair::Key("this.is.not.set"))));
}

TEST_F(TestPassThroughConfiguratorFixture, TypeCheckedInteraction)
{
    Configurator conf(&mStorage, &mTransactions);
    PassThroughConfigurator passer(&conf);

    ASSERT_SET_SUCCESS_NEW_KEY(passer.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0));
    ASSERT_SET_SUCCESS_NEW_KEY(passer.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0));
    ASSERT_SET_SUCCESS_NEW_KEY(passer.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0));
    passer.Commit(0);

    ASSERT_SET_SUCCESS(passer.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle2")), 0));
    ASSERT_SET_SUCCESS(passer.Set(ConfigPair("wireless.cheese", ConfigType<bool>(false)), 0));
    ASSERT_SET_SUCCESS(passer.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(25)), 0));
    passer.Commit(0);

    ASSERT_SET_SUCCESS_NEW_KEY(passer.Set(ConfigPair("wireless.ssid", ConfigType<int32_t>(5)), 0));
    ASSERT_SET_SUCCESS_NEW_KEY(passer.Set(ConfigPair("wireless.ssid", ConfigType<std::string>("true")), 0));
    passer.Commit(0);
    ASSERT_SET_TYPE_MISMATCH(passer.Set(ConfigPair("wireless.ssid", ConfigType<bool>(true)), 0));
}

TEST_F(TestPassThroughConfiguratorFixture, TestDrop)
{
    Configurator conf(&mStorage, &mTransactions);
    PassThroughConfigurator passer(&conf);

    ASSERT_SET_SUCCESS_NEW_KEY(passer.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0));
    ASSERT_SET_SUCCESS_NEW_KEY(passer.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0));
    passer.Drop(0);
    passer.Commit(0);
    ASSERT_FALSE(mStorage.ValueExists("wireless.waffle"));
    ASSERT_FALSE(mStorage.ValueExists("wireless.cheese"));
}
