#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <log4cxx/logger.h>

#include "HookUser.h"

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_hook_user"));
}

class MockExternalService : public IExternalRunner
{
public:
    MockExternalService()
        : mValues()
        , mCalls(0)
    {
    }

    folly::Future<folly::Unit>
    Run(const IExternalRunner::FormattedValues& values) const
    {
        mValues = values;
        mCalls ++;
        return folly::makeFuture();
    }

public:
    mutable IExternalRunner::FormattedValues mValues;
    mutable int mCalls;
};

TEST(HookUsing, Basic)
{
    auto fulfiller = [](const HookUser::HookContainer& )
    {
        return folly::makeFuture(std::vector<ConfigPair::Key>({ConfigKey("key1")}));
    };
    auto pExternal = std::make_shared<MockExternalService>();
    HookUser hooks("service1",
                   HookType::SERVICE,
                   std::vector<HookConfig>({HookConfig("hook1"), HookConfig("hook2")}), fulfiller,
                   pExternal, 1);
    ASSERT_NO_THROW(hooks.ExecuteExternalProcess());

    ASSERT_TRUE(hooks.HasHook(ConfigPair::Key("hook1")));
    ASSERT_FALSE(hooks.HasHook(ConfigPair::Key("doesn'texist")));
}

TEST(HookUsing, Equality)
{
    auto fulfiller = [](const HookUser::HookContainer& )
    {
        return folly::makeFuture(std::vector<ConfigPair::Key>({ConfigKey("key1")}));
    };
    auto pExternal = std::make_shared<MockExternalService>();
    HookUser hooks("service1",
                   HookType::SERVICE,
                   std::vector<HookConfig>({HookConfig("hook1"), HookConfig("hook2")}), fulfiller,
                   pExternal, 1);

    auto fulfiller2 = [](const HookUser::HookContainer&)
    {
        return folly::makeFuture(std::vector<ConfigPair::Key>());
    };
    HookUser hooks2("service2",
                    HookType::SERVICE,
                    std::vector<HookConfig>({HookConfig("hook1"), HookConfig("hook2")}), fulfiller2,
                    pExternal, 1);
    ASSERT_EQ(hooks, hooks2);

    auto pExternal2 = std::make_shared<MockExternalService>();
    HookUser hooks3("service3",
                    HookType::SERVICE,
                    std::vector<HookConfig>({HookConfig("hook1"), HookConfig("hook2")}), fulfiller2,
                    pExternal2, 1);
    ASSERT_NE(hooks, hooks3);

    HookUser hooks4("service4",
                    HookType::SERVICE,
                    std::vector<HookConfig>({HookConfig("hook1")}), fulfiller2, pExternal, 1);
    ASSERT_NE(hooks, hooks4);
}

TEST(HookUsing, Hashing)
{
    auto fulfiller = [](const HookUser::HookContainer& )
    {
        return folly::makeFuture(std::vector<ConfigPair::Key>({ConfigKey("key1")}));
    };
    auto pExternal = std::make_shared<MockExternalService>();
    HookUser hooks("service1",
                   HookType::SERVICE,
                   std::vector<HookConfig>({HookConfig("hook1"), HookConfig("hook2")}), fulfiller,
                   pExternal, 1);

    auto fulfiller2 = [](const HookUser::HookContainer&)
    {
        return folly::makeFuture(std::vector<ConfigPair::Key>());
    };
    HookUser hooks2("service2",
                    HookType::SERVICE,
                    std::vector<HookConfig>({HookConfig("hook1"), HookConfig("hook2")}), fulfiller2,
                    pExternal, 1);
    HookHasher hasher;
    ASSERT_EQ(hasher(hooks), hasher(hooks2));

    auto pExternal2 = std::make_shared<MockExternalService>();
    HookUser hooks3("service3",
                    HookType::SERVICE,
                    std::vector<HookConfig>({HookConfig("hook1"), HookConfig("hook2")}), fulfiller2,
                    pExternal2, 1);
    ASSERT_NE(hasher(hooks), hasher(hooks3));

    HookUser hooks4("service4",
                    HookType::SERVICE,
                    std::vector<HookConfig>({HookConfig("hook1")}), fulfiller2, pExternal, 1);
    ASSERT_NE(hasher(hooks), hasher(hooks4));
}

TEST(HookUsing, Wildcards)
{
    auto fulfiller = [](const HookUser::HookContainer& )
    {
        return folly::makeFuture(std::vector<ConfigPair::Key>());
    };
    auto pExternal = std::make_shared<MockExternalService>();
    HookUser hooks("service1",
                   HookType::SERVICE,
                   std::vector<HookConfig>({HookConfig("hook.other.*"),
                                            HookConfig("hook.space.*"),
                                            HookConfig("hook.space.waffle")}),
                   fulfiller,
                   pExternal, 1);
    ASSERT_TRUE(hooks.HasHook(ConfigKey("hook.other.waffle.sheets")));
    ASSERT_FALSE(hooks.HasHook(ConfigKey("hook.thing.space")));
    // hooks has the specific hook, but also a namespace that covers it
    ASSERT_TRUE(hooks.HasHook(ConfigKey("hook.space.waffle")));

    // Wildcard at the base namespace just gets everything
    HookUser allhooks("service2",
                      HookType::SERVICE,
                      std::vector<HookConfig>({HookConfig("*")}),
                      fulfiller,
                      pExternal, 1);
    ASSERT_TRUE(allhooks.HasHook(ConfigKey("hook.other.waffle.sheets")));
    ASSERT_TRUE(allhooks.HasHook(ConfigKey("literallyanything")));
    ASSERT_TRUE(allhooks.HasHook(ConfigKey("literally.all.things.i.mean.it")));
}
