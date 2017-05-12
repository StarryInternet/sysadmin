#include <cassert>
#include <atomic>
#include <functional>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include <unistd.h>
#include <uv.h>

#include "Helpers.h"
#include "fixtures/EchoServerFixture.h"
#include "fixtures/EventLoopFixture.h"

// decibel-cpp includes
#include "decibel/niceuv/EventLoop.h"
#include "decibel/niceuv/FutureSubprocess.h"
#include "decibel/niceuv/IdleEvent.h"
#include "decibel/niceuv/InterfaceInfo.h"
#include "decibel/niceuv/SubprocessHandler.h"
#include "decibel/niceuv/TcpConn.h"
#include "decibel/niceuv/TcpServer.h"
#include "decibel/niceuv/TimerEvent.h"
#include "decibel/niceuv/ThreadSync.h"
#include "decibel/niceuv/FileChangeEvent.h"

#include <log4cxx/logger.h>

#include <folly/futures/Promise.h>
#include <folly/futures/Future.h>

using namespace std::placeholders;
using namespace decibel::niceuv;

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_niceuv"));
}

// Fixtures and such
void idle_callback()
{
}

struct TcpConnFixture : public virtual EventLoopFixture
{
    TcpConnFixture()
        : EventLoopFixture()
        , mSendData({'c', 'h', 'e', '\n'})
        , mServer(new EchoServerFixture(&mEventLoop))
    {
    }

    virtual void SetUp()
    {
        EventLoopFixture::SetUp();
        mServer->RunEchoServer(2000);
    }

    virtual void TearDown()
    {
    }

    void ConnectCallback(int status)
    {
        ASSERT_EQ(status, 0);
    }

    void ReadCallback(const TcpConn::Buffer& buf)
    {
        LOG4CXX_INFO(spLogger, "got echoed some data");
        REQUIRE_EQUAL_VECTORS(buf, mSendData);
        mConn->Stop();
        mServer.reset();
    }

    TcpConn::Buffer mSendData;
    TcpConn* mConn;
    std::unique_ptr<EchoServerFixture> mServer;
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

struct SubprocessFixture : public EventLoopFixture
{
    SubprocessFixture()
        : EventLoopFixture()
        , mStdOutData({'c', 'h', 'e', 'e', 's', 'e', '\n'})
        , mCallbackCounter(0)
    {
    }

    ~SubprocessFixture()
    {
    }

    void ReadCallback(const SubprocessHandler::Buffer& buf)
    {
        REQUIRE_EQUAL_VECTORS(buf, mStdOutData);
    }

    void ExitCallback(int status)
    {
        ASSERT_EQ(status, 0);
        mCallbackCounter++;
    }

    SubprocessHandler::Buffer mStdOutData;
    int mCallbackCounter;
};

struct PollerFixture: public EventLoopFixture
{
    PollerFixture()
        : EventLoopFixture()
        , mPipes{-1,-1}
    {
    }

    ~PollerFixture()
    {
    }

    virtual void SetUp()
    {
        EventLoopFixture::SetUp();

        if (pipe(mPipes) < 0)
        {
            FAIL() << "pipe setup failed";
        }
    }

    virtual void TearDown()
    {
        for (auto&& pipe : mPipes)
        {
            if (pipe > 0)
            {
                close(pipe);
            }
        }
    }

    int mPipes[2];
};

struct FileChangeFixture : public EventLoopFixture
{
    FileChangeFixture()
        :   EventLoopFixture()
    {
        m_filepath = std::string(tmpnam(NULL));
    }


    virtual void SetUp()
    {
        EventLoopFixture::SetUp();
    }

    virtual void TearDown()
    {

    }

    std::string m_filepath;
};

TEST_F(EventLoopFixture, TestLoopStartStop)
{
    ASSERT_NO_EVENTS(mEventLoop);
    mEventLoop.Stop();
}

TEST_F(EventLoopFixture, TestIdleEvent)
{
    ASSERT_NO_EVENTS(mEventLoop);
    IdleEvent event(&mEventLoop, idle_callback);

    event.Start();
    ASSERT_EVENTS_REMAIN(mEventLoop);
    event.Stop();
    ASSERT_NO_EVENTS(mEventLoop);
}

TEST(TestInterfaceInfo, Constructor)
{
    InterfaceInfo info("0.0.0.0", 5000);
    ASSERT_EQ(info.GetIpPort(), "0.0.0.0:5000");

    InterfaceInfo info2("192.168.1.1", 65000);
    ASSERT_EQ(info2.GetIpPort(), "192.168.1.1:65000");
    ASSERT_NE(info2.GetIpPort(), "0.0.0.0:5000");
}

TEST_F(TcpConnFixture, TestTcpConnServer)
{
    TcpConn tcp(&mEventLoop);
    tcp.RegisterReadCallback(
        std::bind(&TcpConnFixture::ReadCallback, this, _1));
    tcp.RegisterConnectCallback(
        std::bind(&TcpConnFixture::ConnectCallback, this, _1));
    mConn = &tcp;

    tcp.Connect("127.0.0.1", 2000);
    tcp.Start();

    tcp.Write(mSendData);
    LOOP_UNTIL_COMPLETE(mEventLoop);
    tcp.Stop();
    ASSERT_NO_EVENTS(mEventLoop);
}

TEST_F(TimerFixture, TestTimerEvent)
{
    EventLoopTimer eTimer(&mEventLoop);
    TimerEvent timer(
        &eTimer, 50, std::bind(&TimerFixture::TimerCallback, this));
    timer.Start();

    ASSERT_EVENTS_REMAIN(mEventLoop);
    // The event is raised immediately when it is registered
    ASSERT_EQ(mCallbackCounter, 1);

    ADVANCE_TIME_MS(2);
    ASSERT_EVENTS_REMAIN(mEventLoop);
    ASSERT_EQ(mCallbackCounter, 1);

    // Due to the way we advance time, only one callback will be fired here
    ADVANCE_TIME_MS(100);
    ASSERT_EVENTS_REMAIN(mEventLoop);
    ASSERT_EQ(mCallbackCounter, 2);

    timer.Stop();

    ADVANCE_TIME_MS(100);
    ASSERT_NO_EVENTS(mEventLoop);
    ASSERT_EQ(mCallbackCounter, 2);

    timer.Reschedule(25);
    timer.Start();

    ADVANCE_TIME_MS(30);
    ASSERT_EVENTS_REMAIN(mEventLoop);
    ASSERT_EQ(mCallbackCounter, 3);

    ADVANCE_TIME_MS(30);
    ASSERT_EVENTS_REMAIN(mEventLoop);
    ASSERT_EQ(mCallbackCounter, 4);

    timer.Stop();
    ASSERT_NO_EVENTS(mEventLoop);
    ASSERT_EQ(mCallbackCounter, 4);
}

TEST_F(TimerFixture, TestOneShotTimerEvent)
{
    EventLoopTimer eTimer(&mEventLoop);
    OneShotTimerEvent timer(
        &eTimer, 50, std::bind(&TimerFixture::TimerCallback, this));
    timer.Start();

    ASSERT_EVENTS_REMAIN(mEventLoop);
    ASSERT_EQ(mCallbackCounter, 0);

    ADVANCE_TIME_MS(100);
    ASSERT_NO_EVENTS(mEventLoop);
    ASSERT_EQ(mCallbackCounter, 1);
}

TEST_F(SubprocessFixture, TestBufferedSubprocessHandler)
{
    SubprocessHandler subproc(&mEventLoop, {"echo", "cheese"});
    subproc.RegisterStdOutCallback(
        std::bind(&SubprocessFixture::ReadCallback, this, _1));
    subproc.RunProcess();
    mEventLoop.RunForever();

    subproc.RunProcess();
    subproc.RunProcess();
    mEventLoop.RunForever();

    subproc.RegisterProcessExitCallback(
        std::bind(&SubprocessFixture::ExitCallback, this, _1));
    subproc.RunProcess();
    ASSERT_TRUE(subproc.Running());
    mEventLoop.RunForever();
    ASSERT_EQ(mCallbackCounter, 1);
}

TEST_F(SubprocessFixture, TestStreamingSubprocessHandler)
{
    SubprocessHandler subproc(&mEventLoop, {"echo", "cheese"});
    subproc.RegisterStdOutCallback(
        std::bind(&SubprocessFixture::ReadCallback, this, _1), true);
    subproc.RunProcess();
    mEventLoop.RunForever();
}

TEST_F(SubprocessFixture, TestFutureSubprocessHandler)
{
    FutureSubprocess subproc(&mEventLoop, {"echo", "cheesy future"});
    int called = 0;
    subproc.RunSubprocess()
        .then([&](SubprocessHandler::Buffer buffer) {
            REQUIRE_EQUAL_VECTORS(std::vector<char>({'c',
                                                     'h',
                                                     'e',
                                                     'e',
                                                     's',
                                                     'y',
                                                     ' ',
                                                     'f',
                                                     'u',
                                                     't',
                                                     'u',
                                                     'r',
                                                     'e',
                                                     '\n'}),
                                  buffer);
            mEventLoop.Stop();
            called++;
        })
        .onError([](const FutureSubprocessError&) {
            FAIL() << "Should not have errored out";
        });
    mEventLoop.RunForever();
    ASSERT_EQ(1, called);
}

TEST_F(SubprocessFixture, TestFutureSubprocessHandlerFailure)
{
    FutureSubprocess subproc(&mEventLoop, {"ls", "/does/not/exist"});
    subproc.RunSubprocess()
        .then([&](SubprocessHandler::Buffer) {
            FAIL() << "Should not have received any data";
            mEventLoop.Stop();
        })
        .onError([&](const FutureSubprocessError& err) {
            LOG4CXX_ERROR(spLogger, "Subprocess error: " << err.what());
            mEventLoop.Stop();
        });
    mEventLoop.RunForever();
}

TEST_F(SubprocessFixture, TestConstructionDeletion)
{
    SubprocessHandler subproc(&mEventLoop, {"echo", "cheese"});
    subproc.RegisterStdOutCallback(
        std::bind(&SubprocessFixture::ReadCallback, this, _1), true);
    subproc.RunProcess();
}

TEST_F(SubprocessFixture, TestFutureConstructionDeletion)
{
    FutureSubprocess subproc(&mEventLoop, {"echo", "waffle future"});
    subproc.RunSubprocess()
        .then([&](SubprocessHandler::Buffer) {
            FAIL() << "Loop never runs so this is impossible";
        })
        .onError([](const FutureSubprocessError&) {
            FAIL() << "Should not have errored out";
        });
}

TEST_F(PollerFixture, TestPollerRead)
{
    std::string data = "123";
    Poller poller(&mEventLoop, mPipes[0], &data);

    poller.RegisterReadableCallback([&](int status, void *context) {
        ASSERT_EQ(*reinterpret_cast<std::string*>(context), data);
        ASSERT_EQ(status, 0);
        mEventLoop.Stop();
    });
    poller.Start(Poller::PollerEvent::read);
    write(mPipes[1], data.c_str(), data.size());
    mEventLoop.RunForever();
}

TEST_F(PollerFixture, TestPollerNeverReadable)
{
    Poller poller(&mEventLoop, mPipes[0]);
    EventLoopTimer timer(&mEventLoop);
    OneShotTimerEvent stopper(&timer, 50, [&](){ mEventLoop.Stop(); });

    stopper.Start();
    poller.RegisterReadableCallback([&](int, void*) {
        FAIL();
    });
    poller.Start(Poller::PollerEvent::read);
    mEventLoop.RunForever();
}

TEST_F(PollerFixture, TestPollerStopAndRead)
{
    size_t counter = 0;
    const size_t data_size = 3;
    char data[data_size];
    Poller poller(&mEventLoop, mPipes[0], &mPipes[0]);
    EventLoopTimer timer(&mEventLoop);
    OneShotTimerEvent stopper(&timer, 50, [&](){ mEventLoop.Stop(); });

    poller.RegisterReadableCallback([&](int status, void *context) {
        char buffer[data_size];

        ASSERT_EQ(status, 0);
        read(*reinterpret_cast<int*>(context), buffer, data_size);
        ++counter;
    });

    poller.Start(Poller::PollerEvent::read);
    write(mPipes[1], data, data_size);
    stopper.Start();
    mEventLoop.RunForever();

    ASSERT_EQ(counter, 1);

    poller.Stop();
    write(mPipes[1], data, data_size);
    stopper.Start();
    mEventLoop.RunForever();

    ASSERT_EQ(counter, 1);
}

TEST_F(FileChangeFixture, TestChangeFile)
{
    FileChangeEvent changepoller(m_filepath, &mEventLoop);

    changepoller.RegisterChangeCallback([&](uv_fs_event_t* handle, int, int){
        size_t bufsz = 100;
        char buffer[bufsz];
        uv_fs_event_getpath(handle, buffer, &bufsz);

        EXPECT_EQ(std::string(buffer), m_filepath);
    });


    changepoller.Start();

    std::ofstream tmp(m_filepath);
    tmp << "._.";

    mEventLoop.RunForever();
}

TEST_F(FileChangeFixture, TestNoChangeFile)
{
    FileChangeEvent changepoller(m_filepath, &mEventLoop);

    changepoller.RegisterChangeCallback([&](uv_fs_event_t*, int, int){
        FAIL();
    });

    changepoller.Start();

    std::ofstream tmp(m_filepath);
    tmp.close();

    mEventLoop.RunForever();

}

TEST_F(EventLoopFixture, TestQueueWork)
{
    std::atomic<int> counter(0);

    mEventLoop.QueueWork(
        [&](void *context) {
            EXPECT_EQ(counter.fetch_add(1), 0);
            EXPECT_EQ(context, nullptr);
        },
        [&](int status, void *context) {
            EXPECT_EQ(status, 0);
            EXPECT_EQ(context, nullptr);
            mEventLoop.Stop();
        });

    mEventLoop.RunForever();

    ASSERT_EQ(counter.load(), 1);
}

TEST_F(EventLoopFixture, TestCancelQueuedWork)
{
    EventLoopTimer timer(&mEventLoop);
    bool after_work_called = false;

    auto cancel = mEventLoop.QueueWork(
        [](void*) {
            FAIL() << "shouldn't be called";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        },
        [&](int status, void*) {
            EXPECT_EQ(status, UV_ECANCELED);
            after_work_called = true;
        });

    cancel.Cancel();

    OneShotTimerEvent timeout(&timer, 100, [&](){
        mEventLoop.Stop();
    });

    mEventLoop.RunForever();

    ASSERT_TRUE(after_work_called);
}

TEST_F(EventLoopFixture, TestCancellingQueuedWorkDoesntExplode)
{
    EventLoopTimer timer(&mEventLoop);
    int context = 5;

    auto cancel = mEventLoop.QueueWork(
        [](void *context) {
            EXPECT_EQ(*(int *)context, 5);
            std::this_thread::sleep_for(std::chrono::seconds(2));
        },
        [](int,void*){},
        &context);

    OneShotTimerEvent timeout(&timer, 100, [&](){
        try
        {
            cancel.Cancel();
            FAIL();
        }
        catch(std::exception&)
        {
        }

        mEventLoop.Stop();
    });

    timeout.Start();

    mEventLoop.RunForever();
}

TEST_F(EventLoopFixture, TestMutex)
{
    EventLoopTimer timer(&mEventLoop);
    std::atomic<int> counter(0);
    UvMutex lock;

    mEventLoop.QueueWork(
        [&](void *context) {
            UvMutexGuard guard(reinterpret_cast<UvMutex*>(context));
            counter.fetch_add(1);
        },
        [&](int status, void *context) {
            ASSERT_EQ(status, 0);

            auto test_mutex = reinterpret_cast<UvMutex*>(context);

            ASSERT_EQ(test_mutex->TryLock(), 0);
            test_mutex->Unlock();
        },
        &lock);

    OneShotTimerEvent assert_zero(&timer, 200, [&]() {
        EXPECT_EQ(counter.load(), 0);
    });

    OneShotTimerEvent unlock(&timer, 300, [&]() {
        lock.Unlock();
    });

    OneShotTimerEvent stop(&timer, 400, [&]() {
        mEventLoop.Stop();
    });

    ASSERT_EQ(lock.TryLock(), 0);

    assert_zero.Start();
    unlock.Start();

    mEventLoop.RunForever();

    ASSERT_EQ(lock.TryLock(), 0);
    ASSERT_EQ(counter.load(), 1);

    lock.Unlock();
}

TEST(UvCondVar, TestCondVar)
{
    UvMutex lock;
    UvMutex *lock_ptr = &lock;
    UvCondVar condvar;
    std::atomic<int> counter(0);

    auto t1 = std::thread([&condvar, &counter, lock_ptr]() {
        lock_ptr->Lock();
        condvar.Wait(lock_ptr);
        counter.fetch_add(1);
        lock_ptr->Unlock();
    });

    auto t2 = std::thread([&condvar, &counter, lock_ptr]() {
        lock_ptr->Lock();
        condvar.Wait(lock_ptr);
        counter.fetch_add(1);
        lock_ptr->Unlock();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    EXPECT_EQ(counter.load(), 0);

    condvar.Signal();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    EXPECT_EQ(counter.load(), 1);

    condvar.Signal();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    EXPECT_EQ(counter.load(), 2);

    t1.join();
    t2.join();
}

TEST_F(EventLoopFixture, TestEventLoopAsExecutor)
{
    folly::Promise<int> p;
    auto main_thread_id = std::this_thread::get_id();
    std::atomic<std::thread::id> pthread_id;
    std::atomic<int> counter(0);
    EventLoopTimer timer(&mEventLoop);
    OneShotTimerEvent ref(&timer, 100, [&]() { counter.fetch_add(1); });

    auto pthread = std::thread([&]() {
        pthread_id.store(std::this_thread::get_id());
        p.setValue(1);
    });

    p.getFuture()
        .then([&](int value) {
            EXPECT_EQ(pthread_id.load(), std::this_thread::get_id());
            EXPECT_EQ(value, 1);
            counter.fetch_add(1);
            return value;
        })
        .via(&mEventLoop)
        .then([&](int value) {
            EXPECT_EQ(main_thread_id, std::this_thread::get_id());
            EXPECT_EQ(value, 1);
            counter.fetch_add(1);
        });

    ref.Start();

    mEventLoop.RunForever();

    pthread.join();

    ASSERT_EQ(counter.load(), 3);
}

TEST_F(EventLoopFixture, DnsResolutionSuccess)
{
    // My website is a static IP as of writing this...
    mEventLoop.ResolveHostname("ericbrown23.com").then(
        [&](const std::vector<std::string>& addrs)
        {
            REQUIRE_EQUAL_VECTORS(std::vector<std::string>({"107.170.77.44"}),
                                  addrs);
            mEventLoop.Stop();
        });
    mEventLoop.RunForever();
}

TEST_F(EventLoopFixture, DnsResolutionMultipleAnswers)
{
    mEventLoop.ResolveHostname("google.com").then(
        [&](const std::vector<std::string>& addrs)
        {
            ASSERT_LE(1, addrs.size());
            mEventLoop.Stop();
        });
    mEventLoop.RunForever();
}

TEST_F(EventLoopFixture, DnsFailure)
{
    mEventLoop.ResolveHostname("google.nottld").then(
        [&](const std::vector<std::string>& addrs)
        {
            LOG4CXX_ERROR(spLogger, "Should not be called");
            mEventLoop.Stop();
            FAIL();
        }).onError(
        [&](const DnsResolutionError& err)
        {
            mEventLoop.Stop();
        });
    mEventLoop.RunForever();
}

TEST_F(EventLoopFixture, DnsTimeout)
{
    mEventLoop.ResolveHostname(
        "google.nottld",
        std::chrono::milliseconds(0)
    ).then(
        [&](const std::vector<std::string>& addrs)
        {
            LOG4CXX_ERROR(spLogger, "Should not be called");
            mEventLoop.Stop();
            FAIL();
        }).onError(
        [&](const DnsResolutionError& err)
        {
            mEventLoop.Stop();
        }
    );
    mEventLoop.RunForever();
}

TEST_F(EventLoopFixture, DnsMultipleRequests)
{
    mEventLoop.ResolveHostname("google.com").then(
        [&](const std::vector<std::string>& addrs)
        {
            ASSERT_LE(1, addrs.size());
        }).onError(
        [&](const DnsResolutionError& err)
        {
            mEventLoop.Stop();
            FAIL();
        });

    mEventLoop.ResolveHostname("ericbrown23.com").then(
        [&](const std::vector<std::string>& addrs)
        {
            REQUIRE_EQUAL_VECTORS(std::vector<std::string>({"107.170.77.44"}),
                                  addrs);
        }).onError(
        [&](const DnsResolutionError& err)
        {
            mEventLoop.Stop();
            FAIL();
        });

    mEventLoop.RunForever();
}
