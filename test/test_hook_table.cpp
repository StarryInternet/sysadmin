#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <log4cxx/logger.h>

#include "HookTable.h"

#include "TemplaterRunner.h"

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_hook_table"));
}

class HookTableFixture : public ::testing::Test
{
public:
    HookTableFixture()
        : ::testing::Test()
        , mTable()
    {
        mTable.Insert(HookUser("template1",
                               HookType::TEMPLATER,
                               {},
                               NULL,
                               NULL,
                               1));
        mTable.Insert(HookUser("service1",
                               HookType::SERVICE,
                               {},
                               NULL,
                               NULL,
                               1));
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    HookTable mTable;
};

TEST_F(HookTableFixture, Insert)
{
    ASSERT_EQ(1, mTable.ByType(HookType::TEMPLATER).size());
    ASSERT_EQ(1, mTable.ByType(HookType::SERVICE).size());
    mTable.Insert(HookUser("template2",
                           HookType::TEMPLATER,
                           {},
                           NULL,
                           NULL,
                           1));
    ASSERT_EQ(2, mTable.ByType(HookType::TEMPLATER).size());
    ASSERT_EQ(1, mTable.ByType(HookType::SERVICE).size());
    mTable.Insert(HookUser("service2",
                           HookType::SERVICE,
                           {},
                           NULL,
                           NULL,
                           1));
    ASSERT_EQ(2, mTable.ByType(HookType::SERVICE).size());
    ASSERT_EQ(2, mTable.ByType(HookType::TEMPLATER).size());
}

TEST_F(HookTableFixture, ByName)
{
    // these macros sometimes can't resolve optional's implicit bool conversion
    ASSERT_TRUE(mTable.ByName("template1") && true);
    ASSERT_TRUE(mTable.ByName("service1") && true);
    ASSERT_FALSE(mTable.ByName("nonExist") && true);
}

TEST_F(HookTableFixture, ByType)
{
    ASSERT_EQ(1, mTable.ByType(HookType::TEMPLATER).size());
    ASSERT_EQ(1, mTable.ByType(HookType::SERVICE).size());
}

TEST_F(HookTableFixture, GetOrderedServices)
{
    mTable.Insert(HookUser("service3",
                           HookType::SERVICE,
                           {},
                           NULL,
                           NULL,
                           3));
    mTable.Insert(HookUser("service2",
                           HookType::SERVICE,
                           {},
                           NULL,
                           NULL,
                           2));
    mTable.Insert(HookUser("service4",
                           HookType::SERVICE,
                           {},
                           NULL,
                           NULL,
                           4));
    auto hooks = mTable.GetOrderedServices();
    ASSERT_EQ(1, hooks[1].at(0)->RunLevel());
    ASSERT_EQ(2, hooks[2].at(0)->RunLevel());
    ASSERT_EQ(3, hooks[3].at(0)->RunLevel());
    ASSERT_EQ(4, hooks[4].at(0)->RunLevel());
}

TEST_F(HookTableFixture, RunLevelOrder)
{
    mTable.Insert(HookUser("service3",
                           HookType::SERVICE,
                           {},
                           NULL,
                           NULL,
                           3));
    mTable.Insert(HookUser("service2",
                           HookType::SERVICE,
                           {},
                           NULL,
                           NULL,
                           2));
    mTable.Insert(HookUser("service4",
                           HookType::SERVICE,
                           {},
                           NULL,
                           NULL,
                           4));
    std::vector<std::shared_ptr<const HookUser>> hooks =
        mTable.RunLevelOrder(HookType::SERVICE);
    ASSERT_EQ(1, hooks.at(0)->RunLevel());
    ASSERT_EQ(2, hooks.at(1)->RunLevel());
    ASSERT_EQ(3, hooks.at(2)->RunLevel());
    ASSERT_EQ(4, hooks.at(3)->RunLevel());
}
