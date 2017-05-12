#include "decibel/niceuv/Poller.h"

#include <log4cxx/logger.h>

#include <uv.h>

#include "decibel/niceuv/EventLoop.h"
#include "decibel/stdlib.h"

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("Poller"));
}

namespace decibel
{
namespace niceuv
{

Poller::Poller(EventLoop *loop, int fd, void *context)
    : mLoop(loop)
    , mFD(fd)
    , mContext(context)
    , mReadableCallback()
{
    mHandle = std::make_unique<UvHandle<uv_poll_t>>();
    mHandle->Get()->data = this;
    mLoop->RegisterPoller(this);
}

Poller::~Poller()
{
}

void Poller::RegisterReadableCallback(ReadableCallback readable_cb)
{
    mReadableCallback = std::move(readable_cb);
}

void Poller::Start(PollerEvent event)
{
    int events = 0;

    switch (event)
    {
    case PollerEvent::read:
        events = UV_READABLE;
        break;
    default:
        throw std::runtime_error("unknown event type");
    }

    uv_poll_start(mHandle->Get(), events, &on_poll_event);
}

void Poller::Stop()
{
    uv_poll_stop(mHandle->Get());
}

void Poller::on_poll_event(uv_poll_t *handle, int status, int events)
{
    reinterpret_cast<Poller*>(handle->data)->PollerEventCallback(status, events);
}

void Poller::PollerEventCallback(int status, int events)
{
    switch (events)
    {
    case UV_READABLE:
        if (mReadableCallback)
        {
            (*mReadableCallback)(status, mContext);
        }
        break;
    default:
        LOG4CXX_ERROR(spLogger, "unknown event type: " << events);
        break;
    }
}

} // namespace niceuv
} // namespace decibel
