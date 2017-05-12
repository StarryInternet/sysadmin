
#pragma once

#include "BaseTimerEvent.h"

namespace decibel
{
namespace niceuv
{
class TimerEvent : public BaseTimerEvent
{
public:
    TimerEvent(ITimer* timer, ssize_t interval, TimerCallback timercb);

    virtual ~TimerEvent();

    void Reschedule(ssize_t interval);
};
}
}
