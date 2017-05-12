
#pragma once

#include "BaseTimerEvent.h"

namespace decibel
{
namespace niceuv
{
class OneShotTimerEvent : public BaseTimerEvent
{
public:
    OneShotTimerEvent(ITimer* timer, ssize_t timeout, TimerCallback timercb);

    OneShotTimerEvent(ITimer* timer, ssize_t timeout);

    ~OneShotTimerEvent();
};
}
}
