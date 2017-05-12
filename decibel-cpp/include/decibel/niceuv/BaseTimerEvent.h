
#pragma once

#include <functional>

#include "ITimer.h"

namespace decibel
{
namespace niceuv
{
class BaseTimerEvent
{
public:
    typedef std::function<void()> TimerCallback;

public:
    BaseTimerEvent(ITimer* timer,
                   ssize_t timeout,
                   ssize_t interval,
                   TimerCallback timercb);

    BaseTimerEvent(ITimer* timer, ssize_t timeout, ssize_t interval);

    virtual ~BaseTimerEvent();

    void Start();

    void Stop();

    void SetCallback(TimerCallback&& callback);

protected:
    ITimer* mpTimer;
    ssize_t mTimeout;
    ssize_t mInterval;
    ITimer::TimerId mTimerId;

private:
    TimerCallback mTimerCb;
};
}
}
