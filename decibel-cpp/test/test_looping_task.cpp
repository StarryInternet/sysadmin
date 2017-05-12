#include <gtest/gtest.h>

#include "decibel/messaging/LoopingTask.h"
#include "decibel/messaging/Reactor.h"

#include <log4cxx/logger.h>

using namespace decibel::messaging;

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_looping_task"));
}

struct LoopingTaskFixture : public ::testing::Test
{
    LoopingTaskFixture() : mReactor(), mCalls(0)
    {
    }

    void Callback()
    {
        mCalls++;
    }

    Reactor mReactor;
    size_t mCalls;
};

TEST_F(LoopingTaskFixture, Construction)
{
    LoopingTask task(&mReactor, std::bind(&LoopingTaskFixture::Callback, this));
    ASSERT_FALSE(task.Running());
    ASSERT_EQ(0, mCalls);
}

TEST_F(LoopingTaskFixture, StopWithNoStart)
{
    LoopingTask task(&mReactor, std::bind(&LoopingTaskFixture::Callback, this));
    task.Stop();
    ASSERT_FALSE(task.Running());
    ASSERT_EQ(0, mCalls);
}

TEST_F(LoopingTaskFixture, StartStopNoLoop)
{
    LoopingTask task(&mReactor, std::bind(&LoopingTaskFixture::Callback, this));
    task.Start(1);
    ASSERT_TRUE(task.Running());
    task.Stop();
    ASSERT_FALSE(task.Running());
    ASSERT_EQ(0, mCalls);
}

TEST_F(LoopingTaskFixture, StartLoopStop)
{
    LoopingTask task(&mReactor, std::bind(&LoopingTaskFixture::Callback, this));
    task.Start(50);
    mReactor.CallLater(200, [&]() {
        ASSERT_TRUE(task.Running());
        task.Stop();
    });
    mReactor.Start();
    ASSERT_FALSE(task.Running());
    ASSERT_GT(mCalls, 0);
}

TEST_F(LoopingTaskFixture, FulfillsFutureOnStop)
{
    LoopingTask task(&mReactor, std::bind(&LoopingTaskFixture::Callback, this));
    auto fut = task.Start(1);
    bool futureCalled = false;
    bool errCalled = false;
    fut.then([&]() { futureCalled = true; })
        .onError([&](const std::exception&) { errCalled = true; });
    mReactor.CallLater(10, [&]() {
        ASSERT_TRUE(task.Running());
        task.Stop();
    });
    mReactor.Start();
    ASSERT_TRUE(futureCalled);
    ASSERT_FALSE(errCalled);
}

TEST_F(LoopingTaskFixture, FulfillsFutureOnError)
{
    LoopingTask task(&mReactor, []() { throw std::runtime_error("error"); });
    auto fut = task.Start(1);
    bool futureCalled = false;
    bool errCalled = false;
    fut.then([&]() { futureCalled = true; })
        .onError([&](const std::exception&) { errCalled = true; });
    mReactor.CallLater(10, [&]() {
        // Task should have stopped running when the error was thrown
        ASSERT_FALSE(task.Running());
    });
    mReactor.Start();
    ASSERT_TRUE(errCalled);
    ASSERT_FALSE(futureCalled);
}

TEST_F(LoopingTaskFixture, StopStart)
{
    LoopingTask task(&mReactor, std::bind(&LoopingTaskFixture::Callback, this));
    int futureCalled = 0;
    int errCalled = 0;
    task.Start(50)
        .then([&]() { futureCalled++; })
        .onError([&](const std::exception&) { errCalled++; });
    task.Stop();
    task.Start(50)
        .then([&]() { futureCalled++; })
        .onError([&](const std::exception&) { errCalled++; });
    task.Stop();
    mReactor.Start();
    ASSERT_EQ(2, futureCalled);
    ASSERT_EQ(0, errCalled);
}

TEST_F(LoopingTaskFixture, Reschedule)
{
    LoopingTask task(&mReactor, std::bind(&LoopingTaskFixture::Callback, this));
    bool futureCalled = false;
    task.Start(5).then([&futureCalled]() { futureCalled = true; });
    task.Reschedule(50);
    mReactor.CallLater(10, [&]() {
        ASSERT_TRUE(task.Running());
        task.Stop();
    });

    mReactor.Start();
    ASSERT_TRUE(futureCalled);
}
