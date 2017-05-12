
#include "decibel/niceuv/ITimer.h"
#include "decibel/niceuv/EventLoop.h"
#include "decibel/stdlib.h"

#include <time.h>

#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("ITimer"));
}

namespace decibel
{
namespace niceuv
{
ITimer::~ITimer()
{
}

void on_timer(uv_timer_t* handle)
{
    EventLoopTimer::HandleWrapper* timer =
        reinterpret_cast<EventLoopTimer::HandleWrapper*>(handle->data);
    timer->TimerCalled();
}

EventLoopTimer::EventLoopTimer(EventLoop* loop)
    : mpLoop(loop), mTimerHandles(), mTimerCounter(0)
{
}

ITimer::TimerId
EventLoopTimer::Start(ssize_t timeout, ssize_t interval, TimerCallback cb)
{
    auto handle = std::make_unique<UvHandle<uv_timer_t>>();
    HandleWrapper wrapper(std::move(handle), cb, timeout, interval);
    TimerId id = mTimerCounter++;
    mTimerHandles.emplace(id, std::move(wrapper));

    mTimerHandles.at(id).mHandle->Get()->data = &(mTimerHandles.at(id));
    mpLoop->RegisterTimerEvent(&(mTimerHandles.at(id)));
    uv_timer_start(
        mTimerHandles.at(id).mHandle->Get(), on_timer, timeout, interval);

    return id;
}

void EventLoopTimer::Stop(ITimer::TimerId id)
{
    if (mTimerHandles.count(id))
    {
        uv_timer_stop(mTimerHandles.at(id).mHandle->Get());
        mTimerHandles.erase(id);
    }
}

void EventLoopTimer::SetRepeat(ITimer::TimerId id, ssize_t interval)
{
    if (mTimerHandles.count(id))
    {
        mTimerHandles.at(id).mInterval = interval;
        uv_timer_set_repeat(mTimerHandles.at(id).mHandle->Get(), interval);
    }
}

ssize_t EventLoopTimer::CurrentTime() const
{
    return time(0);
}

DebugTimer::DebugTimer() : mTimerHandles(), mCurrentTime(0), mTimerCounter(0)
{
}

ITimer::TimerId
DebugTimer::Start(ssize_t timeout, ssize_t interval, TimerCallback cb)
{
    TimerId id = mTimerCounter++;

    CallbackWrapper wrapper(cb, timeout, interval, mCurrentTime);
    mTimerHandles.emplace(id, std::move(wrapper));

    return id;
}

void DebugTimer::Stop(ITimer::TimerId id)
{
    if (mTimerHandles.count(id))
    {
        mTimerHandles.erase(id);
    }
}

void DebugTimer::SetRepeat(ITimer::TimerId id, ssize_t interval)
{
    if (mTimerHandles.count(id))
    {
        mTimerHandles.at(id).mInterval = interval;
    }
}

void DebugTimer::AdvanceTime(ITimer::milliseconds delta)
{
    ITimer::milliseconds old_time = mCurrentTime;

    for (ssize_t i = old_time.count(); i < (old_time + delta).count(); i++)
    {
        mCurrentTime += ITimer::milliseconds(1);
        ProcessTimeChange();
    }
    LOG4CXX_DEBUG(spLogger,
                  "Advanced Time from " << old_time.count() << " to "
                                        << mCurrentTime.count());
}

void DebugTimer::ProcessTimeChange()
{
    std::vector<ITimer::TimerCallback> fire_list;
    for (auto& handle : mTimerHandles)
    {
        if (!handle.second.mInitiallyFired)
        {
            if ((mCurrentTime - handle.second.mInitialTime).count() >=
                handle.second.mTimeout)
            {
                LOG4CXX_DEBUG(
                    spLogger,
                    "Current time: "
                        << mCurrentTime.count()
                        << " timeout: "
                        << handle.second.mTimeout
                        << " delta: "
                        << (mCurrentTime - handle.second.mInitialTime).count());
                LOG4CXX_DEBUG(
                    spLogger,
                    "Scheduling initial cb with id: " << handle.first);
                fire_list.push_back(handle.second.mTimerCb);
                handle.second.mInitiallyFired = true;
                handle.second.mLastCalled = mCurrentTime;
            }
        }
        else
        {
            if ((mCurrentTime - handle.second.mLastCalled).count() >=
                handle.second.mInterval)
            {
                LOG4CXX_DEBUG(
                    spLogger,
                    "Current time: "
                        << mCurrentTime.count()
                        << " interval: "
                        << handle.second.mInterval
                        << " delta: "
                        << (mCurrentTime - handle.second.mLastCalled).count());
                LOG4CXX_DEBUG(
                    spLogger,
                    "Scheduling interval cb with id: " << handle.first);
                fire_list.push_back(handle.second.mTimerCb);
                handle.second.mLastCalled = mCurrentTime;
            }
        }
    }

    for (auto& cb : fire_list)
    {
        cb();
    }
}

ssize_t DebugTimer::CurrentTime() const
{
    return std::chrono::duration_cast<ITimer::seconds>(mCurrentTime).count();
}
}
}
