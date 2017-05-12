
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <log4cxx/logger.h>

#include "KeyValueFormatter.h"

#define REQUIRE_EQUAL_VECTORS(a, b) EXPECT_THAT((a), ::testing::ContainerEq(b));

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_key_value_formatter"));
}

TEST(KeyValueFormatting, Basic)
{
    auto cmd1 = FormatKeys({ConfigPair("key", ConfigType<bool>(true))});
    REQUIRE_EQUAL_VECTORS(std::vector<std::string>({"key"}), cmd1);

    ConfigPairList pairs = {ConfigPair("listkey", ConfigType<bool>(true))};
    auto cmd2 = FormatKeys(pairs);
    REQUIRE_EQUAL_VECTORS(std::vector<std::string>({"listkey"}), cmd2);

    ConfigPairList pairs2 = {ConfigPair("strlist", ConfigType<bool>(true)),
                             ConfigPair("intlist", ConfigType<bool>(true)),
                             ConfigPair("bool", ConfigType<bool>(true))};
    auto cmd3 = FormatKeys(pairs2);
    REQUIRE_EQUAL_VECTORS(std::vector<std::string>({"strlist", "intlist", "bool"}), cmd3);

    ConfigPairList pairs3 = {ConfigPair("longer.key.com", ConfigType<bool>(true)),
                             ConfigPair("short.key", ConfigType<bool>(true))};
    auto cmd4 = FormatKeys(pairs3);
    REQUIRE_EQUAL_VECTORS(std::vector<std::string>({"longer.key.com", "short.key"}), cmd4);
}
