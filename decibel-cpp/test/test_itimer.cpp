#include <cassert>
#include <functional>
#include <gtest/gtest.h>
#include <iostream>

#include "Helpers.h"

#include "decibel/niceuv/EventLoop.h"
#include "decibel/niceuv/ITimer.h"
#include "decibel/niceuv/IdleEvent.h"
#include "decibel/niceuv/InterfaceInfo.h"
#include "decibel/niceuv/TcpConn.h"
#include "decibel/niceuv/TcpServer.h"
#include "decibel/niceuv/TimerEvent.h"

#include <log4cxx/logger.h>

using namespace std::placeholders;
using namespace decibel::niceuv;

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_itimer"));
}

struct EventLoopFixture : public virtual ::testing::Test
{
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
        mEventLoop.RunForever();
    }

    EventLoop mEventLoop;
};

struct TimerFixture : public EventLoopFixture
{
    TimerFixture() : EventLoopFixture(), mCallbackCounter(0)
    {
    }

    void TimerCallback()
    {
        mCallbackCounter++;
    }

    int mCallbackCounter;
};

TEST_F(TimerFixture, EventLoopTimerTesting)
{
    EventLoopTimer timer(&mEventLoop);

    timer.Start(0, 0, std::bind(&TimerFixture::TimerCallback, this));
    timer.Start(0, 0, std::bind(&TimerFixture::TimerCallback, this));
    timer.Start(0, 0, std::bind(&TimerFixture::TimerCallback, this));
    timer.Start(0, 0, std::bind(&TimerFixture::TimerCallback, this));

    mEventLoop.LoopOnce();

    ASSERT_EQ(mCallbackCounter, 4);
}

TEST_F(TimerFixture, DebugTimerTesting)
{
    DebugTimer timer;

    auto id1 =
        timer.Start(10, 6, std::bind(&TimerFixture::TimerCallback, this));

    EXPECT_EQ(mCallbackCounter, 0);
    timer.AdvanceTime(ITimer::milliseconds(5));
    EXPECT_EQ(mCallbackCounter, 0);

    timer.AdvanceTime(ITimer::milliseconds(10));
    EXPECT_EQ(mCallbackCounter, 1);

    timer.Stop(id1);

    timer.AdvanceTime(ITimer::milliseconds(10));
    EXPECT_EQ(mCallbackCounter, 1);

    auto id2 = timer.Start(5, 5, std::bind(&TimerFixture::TimerCallback, this));
    auto id3 =
        timer.Start(15, 10, std::bind(&TimerFixture::TimerCallback, this));

    EXPECT_EQ(mCallbackCounter, 1);
    timer.AdvanceTime(ITimer::milliseconds(10));
    EXPECT_EQ(mCallbackCounter, 3);
    timer.AdvanceTime(ITimer::milliseconds(5));
    EXPECT_EQ(mCallbackCounter, 5);
    timer.AdvanceTime(ITimer::milliseconds(10));
    EXPECT_EQ(mCallbackCounter, 8);

    timer.SetRepeat(id2, 25);
    EXPECT_EQ(mCallbackCounter, 8);
    timer.AdvanceTime(ITimer::milliseconds(30));
    EXPECT_EQ(mCallbackCounter, 12);

    timer.Stop(id2);
    timer.Stop(id3);

    timer.AdvanceTime(ITimer::milliseconds(60000));
    EXPECT_EQ(mCallbackCounter, 12);
}
