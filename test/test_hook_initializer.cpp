#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <log4cxx/logger.h>

#include "HookInitializer.h"
#include "decibel/messaging/Reactor.h"

#include "Helpers.h"

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_hook_initializer"));
}

namespace dm = decibel::messaging;

class HookInitFixture : public ::testing::Test
{
public:
    HookInitFixture()
      : ::testing::Test()
      , mBaseConfig()
      , mMockStorage()
    {
        mBaseConfig.mTemplaterPath = "/path/to/templater.py";
        mBaseConfig.mTemplates.push_back(TemplateConfig());
        mBaseConfig.mTemplates[0].mName = "name";
        mBaseConfig.mTemplates[0].mPath = "path";
        mBaseConfig.mTemplates[0].mDestination = "dest";
        mBaseConfig.mTemplates[0].mHooks.push_back(HookConfig("hook"));

        mBaseConfig.mServices.push_back(ServiceConfig());
        mBaseConfig.mServices[0].mName = "name1";
        mBaseConfig.mServices[0].mPath = "path";
        mBaseConfig.mServices[0].mHooks.push_back(HookConfig("hook"));
        mBaseConfig.mServices[0].mRunLevel = 1;

        mMockStorage.Set(ConfigPair("hook", ConfigType<std::string>("value")), 0);
        mMockStorage.Set(ConfigPair("wildcards.thing1", ConfigType<std::string>("value")), 0);
        mMockStorage.Set(ConfigPair("wildcards.thing2", ConfigType<std::string>("value")), 0);
        mMockStorage.Set(ConfigPair("wildcards.nested.thing1", ConfigType<std::string>("value")), 0);
        mMockStorage.Set(ConfigPair("wildcards.nested.thing2", ConfigType<std::string>("value")), 0);
        mMockStorage.Set(ConfigPair("wildcards.other.thing1", ConfigType<std::string>("value")), 0);
        mMockStorage.Commit(0);
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    FullHooksConfig mBaseConfig;
    MockSystemStorage mMockStorage;
};

TEST_F(HookInitFixture, Basic)
{
    dm::Reactor r;
    auto mgmr = HookManagerFactory(mBaseConfig, &r, &mMockStorage, 100);

    ASSERT_EQ(1, mgmr->NumberOfTemplaters());
    ASSERT_EQ(1, mgmr->NumberOfServices());

    mBaseConfig.mTemplates.push_back(TemplateConfig());
    mBaseConfig.mTemplates[1].mName = "name2";
    mBaseConfig.mTemplates[1].mPath = "path";
    mBaseConfig.mTemplates[1].mDestination = "dest";
    mBaseConfig.mTemplates[1].mHooks.push_back(HookConfig("hook"));

    auto mgmr2 = HookManagerFactory(mBaseConfig, &r, &mMockStorage, 100);
    ASSERT_EQ(2, mgmr2->NumberOfTemplaters());
    ASSERT_EQ(1, mgmr2->NumberOfServices());
}

TEST_F(HookInitFixture, HookFulfilling)
{
    HookUser::HookContainer hooks;
    hooks.emplace(HookConfig("hook"));
    auto pairs = FulfillHooks(&mMockStorage, hooks).value();

    ASSERT_EQ(1, pairs.size());
    ASSERT_EQ("hook", pairs[0].ToString());

    hooks.emplace("wontfulfill");
    int calledback = 0;
    FulfillHooks(&mMockStorage, hooks).then([](const std::vector<ConfigPair::Key>&)
                                            {
                                                FAIL() << "Should not have succeeded";
                                            }).onError([&calledback](const HookUserError&)
                                            {
                                                calledback++;
                                            });
    ASSERT_EQ(1, calledback);
}

TEST_F(HookInitFixture, HookFulfillingOptionalHooks)
{
    HookUser::HookContainer hooks;
    hooks.emplace(HookConfig("requiredhook"));
    hooks.emplace(HookConfig("optionalhook", true));
    hooks.emplace(HookConfig("optionalhook2", true));
    mMockStorage.Set(ConfigPair("requiredhook", ConfigType<std::string>("bettergetthis")), 0);
    mMockStorage.Set(ConfigPair("optionalhook", ConfigType<std::string>("doesexist")), 0);
    mMockStorage.Commit(0);
    auto pairs = FulfillHooks(&mMockStorage, hooks).value();

    ASSERT_EQ(2, pairs.size());
    ASSERT_EQ("requiredhook", pairs[0].ToString());
    ASSERT_EQ("optionalhook", pairs[1].ToString());
}

TEST_F(HookInitFixture, HookFulfillingWildcards)
{
    HookUser::HookContainer hooks;
    hooks.emplace(HookConfig("wildcards.thing1"));
    hooks.emplace(HookConfig("wildcards.nested.*", true));

    auto keys = FulfillHooks(&mMockStorage, hooks).value();
    ASSERT_EQ(2, keys.size());
}

TEST_F(HookInitFixture, ServiceOrder)
{
    dm::Reactor r;

    mBaseConfig.mServices.push_back(ServiceConfig());
    mBaseConfig.mServices[1].mName = "name";
    mBaseConfig.mServices[1].mPath = "path";
    mBaseConfig.mServices[1].mHooks.push_back(HookConfig("hook"));
    mBaseConfig.mServices[1].mRunLevel = 3;

    mBaseConfig.mServices.push_back(ServiceConfig());
    mBaseConfig.mServices[2].mName = "name";
    mBaseConfig.mServices[2].mPath = "path";
    mBaseConfig.mServices[2].mHooks.push_back(HookConfig("hook"));
    mBaseConfig.mServices[2].mHooks.push_back(HookConfig("hook"));
    mBaseConfig.mServices[2].mRunLevel = 2;

    // We would assert and die if the above services were not properly sorted
    auto mgr = HookManagerFactory(mBaseConfig, &r, &mMockStorage, 100);
}
