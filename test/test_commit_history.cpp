#include <boost/date_time/posix_time/posix_time.hpp>
#include <log4cxx/logger.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "CommitHistory.h"

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_commit_history"));
}

#define REQUIRE_EQUAL_VECTORS(a, b) EXPECT_THAT((a), ::testing::ContainerEq(b));

#define TEST_COMMIT(COMMIT)                                         \
    {                                                               \
        auto seralized = YAMLSerializer(COMMIT);                    \
        ASSERT_TRUE((bool)seralized);                               \
        auto deserialized = YAMLDeserializer(seralized.get());      \
        ASSERT_TRUE((bool)deserialized);                            \
                                                                    \
        CommitHistory commitagain = deserialized.get();             \
                                                                    \
        ASSERT_FALSE(commitagain.mTime.is_not_a_date_time());       \
        ASSERT_EQ(COMMIT.mId, commitagain.mId);                     \
        ASSERT_EQ(COMMIT.mParent, commitagain.mParent);             \
        REQUIRE_EQUAL_VECTORS(COMMIT.mItems, commitagain.mItems);   \
    }


TEST(Serializing, Basic)
{
    CommitHistory commit1;
    commit1.mId = 5;
    commit1.mParent = 4;
    commit1.mTime = boost::posix_time::second_clock::local_time();
    commit1.mItems.emplace_back(ConfigPair("key", ConfigType<std::string>("value")),
                                ConfigPair("key", ConfigType<std::string>("newvalue")));
    commit1.mItems.emplace_back(ConfigPair("key2", ConfigType<bool>(true)),
                                ConfigPair("key2", ConfigType<bool>(false)));
    TEST_COMMIT(commit1);

    CommitHistory commit2;
    commit2.mId = 9;
    commit2.mParent = 8;
    commit2.mTime = boost::posix_time::second_clock::local_time();
    commit2.mItems.emplace_back(ConfigPair("key", ConfigType<int32_t>(4)),
                                ConfigPair("key", ConfigType<int32_t>(9)));
    commit2.mItems.emplace_back(ConfigPair("list", ConfigType<std::vector<int32_t>>({1, 2, 3})),
                                ConfigPair("list", ConfigType<std::vector<int32_t>>({6, 7, 8})));
    TEST_COMMIT(commit2);
}
