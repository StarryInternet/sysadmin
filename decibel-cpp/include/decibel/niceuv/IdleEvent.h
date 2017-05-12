
#pragma once

#include <functional>

#include <uv.h>

#include "UvHandle.h"

namespace decibel
{
namespace niceuv
{
// more forward declaration bs for friend classes
class EventLoop;

class IdleEvent
{
    friend class EventLoop;
    friend void on_idle(uv_idle_t* handle);

public:
    typedef std::function<void()> IdleCallback;

public:
    IdleEvent(EventLoop* loop, IdleCallback idlecb);

    ~IdleEvent();

    void Start();

    void Stop();

private:
    void IdleCalled();

private:
    UvHandle<uv_idle_t> mIdleHandler;
    IdleCallback mIdleCb;
};
}
}
