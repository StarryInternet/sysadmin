#include <gtest/gtest.h>

#include "decibel/niceuv/EventLoop.h"

struct EventLoopFixture : public virtual ::testing::Test
{
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
        mEventLoop.RunForever();
    }

    decibel::niceuv::EventLoop mEventLoop;
};
