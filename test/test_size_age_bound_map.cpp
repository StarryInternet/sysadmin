#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <log4cxx/logger.h>

#include "SizeAgeBoundMap.h"

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_templater_runner"));
}

TEST(SizeAgeBoundMapping, Basic)
{
    SizeAgeBoundMap<int, int> map(5);

    ASSERT_EQ(0, map.Count(5));

    map.Insert(5, 6);
    ASSERT_EQ(6, map.At(5));
    ASSERT_EQ(1, map.Count(5));
    // We don't replace existing items
    ASSERT_FALSE(map.Insert(5, 7));
    ASSERT_EQ(6, map.At(5));
    ASSERT_EQ(1, map.Count(5));

    map.Insert(8, 9);
    ASSERT_EQ(9, map.At(8));

    map.Insert(1, 1);
    map.Insert(2, 2);
    map.Insert(3, 3);
    // At this point the map is maxed out, and should age out the older items
    map.Insert(6, 6);
    ASSERT_EQ(0, map.Count(5));
    map.Insert(7, 7);
    ASSERT_EQ(0, map.Count(8));

    // Erasing should result in us not maxing out the map
    map.Erase(1);
    map.Insert(9, 9);
    // 2 is the oldest item at this point, this shows we didn't age it out
    ASSERT_EQ(1, map.Count(2));
}
