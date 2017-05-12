
#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <ratio>
#include <unordered_map>
#include <uv.h>

#include "UvHandle.h"

namespace decibel
{
namespace niceuv
{
class EventLoop;

class ITimer
{
public:
    typedef std::function<void()> TimerCallback;
    typedef size_t TimerId;
    typedef std::chrono::duration<int> seconds;
    typedef std::chrono::duration<int, std::milli> milliseconds;
    typedef std::chrono::duration<int, std::ratio<60>> minutes;
    typedef std::chrono::duration<int, std::ratio<60 * 60>> hours;

public:
    virtual ~ITimer();

    virtual TimerId
    Start(ssize_t timeout, ssize_t interval, TimerCallback cb) = 0;

    virtual void Stop(TimerId id) = 0;

    virtual void SetRepeat(TimerId id, ssize_t interval) = 0;

    // Current time is the classic seconds since the epoch
    virtual ssize_t CurrentTime() const = 0;
};

class EventLoopTimer : public ITimer
{
public:
    struct HandleWrapper
    {
        HandleWrapper(std::unique_ptr<UvHandle<uv_timer_t>> handle,
                      TimerCallback cb,
                      ssize_t timeout,
                      ssize_t interval)
            : mHandle(std::move(handle))
            , mTimerCb(cb)
            , mTimeout(timeout)
            , mInterval(interval)
        {
        }

        void TimerCalled()
        {
            mTimerCb();
        }

        std::unique_ptr<UvHandle<uv_timer_t>> mHandle;
        TimerCallback mTimerCb;
        ssize_t mTimeout;
        ssize_t mInterval;
    };

public:
    EventLoopTimer(EventLoop* loop);

    // ITimer
public:
    TimerId Start(ssize_t timeout, ssize_t interval, TimerCallback cb);

    void Stop(TimerId id);

    void SetRepeat(TimerId id, ssize_t interval);

    ssize_t CurrentTime() const;

private:
    void TimerCalled();

private:
    EventLoop* mpLoop;
    std::unordered_map<TimerId, HandleWrapper> mTimerHandles;
    ssize_t mTimerCounter;
};

class DebugTimer : public ITimer
{
public:
    struct CallbackWrapper
    {
        CallbackWrapper(TimerCallback cb,
                        ssize_t timeout,
                        ssize_t interval,
                        ITimer::milliseconds initial)
            : mTimerCb(cb)
            , mTimeout(timeout)
            , mInterval(interval)
            , mInitialTime(initial)
            , mInitiallyFired(false)
            , mLastCalled()
        {
        }

        TimerCallback mTimerCb;
        ssize_t mTimeout;
        ssize_t mInterval;
        ITimer::milliseconds mInitialTime;
        bool mInitiallyFired;
        ITimer::milliseconds mLastCalled;
    };

public:
    DebugTimer();

    // ITimer
public:
    TimerId Start(ssize_t timeout, ssize_t interval, TimerCallback cb);

    void Stop(TimerId id);

    void SetRepeat(TimerId id, ssize_t interval);

    void AdvanceTime(milliseconds delta);

    ssize_t CurrentTime() const;

private:
    void ProcessTimeChange();

private:
    std::unordered_map<TimerId, CallbackWrapper> mTimerHandles;
    ITimer::milliseconds mCurrentTime;
    ssize_t mTimerCounter;
};
}
}
