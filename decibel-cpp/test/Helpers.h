#pragma once

#include <chrono>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <thread>

// utilities
#define REQUIRE_EQUAL_VECTORS(a, b) EXPECT_THAT((a), ::testing::ContainerEq(b));

#define ASSERT_NO_EVENTS(loop) ASSERT_EQ(loop.LoopOnce(), 0)

#define ASSERT_EVENTS_REMAIN(loop) ASSERT_NE(loop.LoopOnce(), 0)

#define LOOP_UNTIL_COMPLETE(loop)                                              \
    while (loop.LoopOnce() != 0)                                               \
    {                                                                          \
    }

#define ADVANCE_TIME_MS(ms)                                                    \
    std::this_thread::sleep_for(std::chrono::milliseconds(ms))
