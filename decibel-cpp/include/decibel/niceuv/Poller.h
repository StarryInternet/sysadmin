#pragma once

#include <functional>
#include <memory>

#include <boost/optional.hpp>

#include <uv.h>

#include "UvHandle.h"

namespace decibel
{
namespace niceuv
{
class EventLoop;

class Poller
{
friend EventLoop;

public:
enum class PollerEvent {
    read
};

typedef std::function<void(int,void*)> ReadableCallback;

public:
    Poller(EventLoop *loop, int fd, void *context = nullptr);

    virtual ~Poller();

    void RegisterReadableCallback(ReadableCallback readable_cb);

    void Start(PollerEvent event);
    void Stop();

private:
    EventLoop *mLoop;
    int mFD;
    void *mContext;
    std::unique_ptr<UvHandle<uv_poll_t>> mHandle;
    boost::optional<ReadableCallback> mReadableCallback;

    static void on_poll_event(uv_poll_t *handle, int status, int events);

    void PollerEventCallback(int status, int events);
}; // class Poller

} // namespace niceuv
} // namespace decibel
