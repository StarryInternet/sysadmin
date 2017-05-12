#include "decibel/niceuv/OneShotTimerEvent.h"

namespace decibel
{
namespace niceuv
{
OneShotTimerEvent::OneShotTimerEvent(ITimer* timer,
                                     ssize_t timeout,
                                     TimerCallback timercb)
    : BaseTimerEvent(timer, timeout, 0, timercb)
{
}

OneShotTimerEvent::OneShotTimerEvent(ITimer* timer, ssize_t timeout)
    : BaseTimerEvent(timer, timeout, 0)
{
}

OneShotTimerEvent::~OneShotTimerEvent()
{
}
}
}
