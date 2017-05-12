
#include <cassert>
#include <iostream>

#include "decibel/niceuv/EventLoop.h"
#include "decibel/niceuv/IdleEvent.h"

namespace decibel
{
namespace niceuv
{
void on_idle(uv_idle_t* handle)
{
    IdleEvent* idler = reinterpret_cast<IdleEvent*>(handle->data);
    idler->IdleCalled();
}

IdleEvent::IdleEvent(EventLoop* loop, IdleEvent::IdleCallback idlecb)
    : mIdleHandler(), mIdleCb(idlecb)
{
    mIdleHandler.Get()->data = this;

    loop->RegisterIdleEvent(this);
}

void IdleEvent::Start()
{
    uv_idle_start(mIdleHandler.Get(), on_idle);
}

void IdleEvent::Stop()
{
    uv_idle_stop(mIdleHandler.Get());
}

IdleEvent::~IdleEvent()
{
}

void IdleEvent::IdleCalled()
{
    assert(mIdleCb);
    mIdleCb();
}
}
}
