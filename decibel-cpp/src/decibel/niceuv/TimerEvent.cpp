#include "decibel/niceuv/TimerEvent.h"

namespace decibel
{
namespace niceuv
{
TimerEvent::TimerEvent(ITimer* timer,
                       ssize_t interval,
                       TimerEvent::TimerCallback timercb)
    : BaseTimerEvent(timer, 0, interval, timercb)
{
}

TimerEvent::~TimerEvent()
{
}

void TimerEvent::Reschedule(ssize_t interval)
{
    mInterval = interval;
    mpTimer->SetRepeat(mTimerId, interval);
}
}
}
