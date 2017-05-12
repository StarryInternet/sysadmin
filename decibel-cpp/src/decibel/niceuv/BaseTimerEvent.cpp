#include "decibel/niceuv/BaseTimerEvent.h"

#include <boost/assert.hpp>

namespace decibel
{
namespace niceuv
{
BaseTimerEvent::BaseTimerEvent(ITimer* timer,
                               ssize_t timeout,
                               ssize_t interval,
                               TimerCallback timercb)
    : mpTimer(timer)
    , mTimeout(timeout)
    , mInterval(interval)
    , mTimerId(0)
    , mTimerCb(timercb)
{
}

BaseTimerEvent::BaseTimerEvent(ITimer* timer, ssize_t timeout, ssize_t interval)
    : BaseTimerEvent(timer, timeout, interval, TimerCallback())
{
}

BaseTimerEvent::~BaseTimerEvent()
{
    Stop();
}

void BaseTimerEvent::Start()
{
    BOOST_ASSERT_MSG(mTimerCb,
                     "Timer callback must be set before calling Start()");
    mTimerId = mpTimer->Start(mTimeout, mInterval, mTimerCb);
}

void BaseTimerEvent::Stop()
{
    mpTimer->Stop(mTimerId);
}

void BaseTimerEvent::SetCallback(TimerCallback&& callback)
{
    mTimerCb = std::move(callback);
}
}
}
