
#include <assert.h>
#include <iostream>

#include "decibel/niceuv/EventLoop.h"
#include "decibel/stdlib.h"

#include <log4cxx/logger.h>

#include <folly/MoveWrapper.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("EventLoop"));
}

namespace decibel
{
namespace niceuv
{
const char* strerror(int code)
{
    return uv_strerror(code);
}

EventLoop::EventLoop()
  : mEventLoop(new uv_loop_t)
  , mResolvers()
  , mResCounter(0)
{
    uv_loop_init(mEventLoop);
}

EventLoop::~EventLoop()
{
    // Loop one more time to ensure everything has been destroyed by libuv
    LoopOnce();
    uv_loop_close(mEventLoop);
    delete mEventLoop;
}

void EventLoop::RunForever()
{
    uv_run(mEventLoop, UV_RUN_DEFAULT);
}

int EventLoop::LoopOnce()
{
    return uv_run(mEventLoop, UV_RUN_NOWAIT);
}

void EventLoop::Stop()
{
    uv_stop(mEventLoop);
}

void EventLoop::RegisterIdleEvent(IdleEvent* idler)
{
    int ret = uv_idle_init(mEventLoop, idler->mIdleHandler.Get());
    idler->mIdleHandler.MakeValid();
    assert(ret == 0);
}

void EventLoop::RegisterTcpConn(TcpConn* conn)
{
    int ret = uv_tcp_init(mEventLoop, conn->mTcpHandler->Get());
    conn->mTcpHandler->MakeValid();
    assert(ret == 0);
}

void EventLoop::RegisterTimerEvent(EventLoopTimer::HandleWrapper* timer)
{
    int ret = uv_timer_init(mEventLoop, timer->mHandle->Get());
    timer->mHandle->MakeValid();
    assert(ret == 0);
}

void EventLoop::RegisterAndSpawnSubprocess(SubprocessHandler* subproc)
{
    uv_pipe_init(mEventLoop, subproc->mPipeHandle->Get(), 0);
    subproc->mPipeHandle->MakeValid();
    uv_pipe_init(mEventLoop, subproc->mErrPipeHandle->Get(), 0);
    subproc->mErrPipeHandle->MakeValid();
    int ret = uv_spawn(
        mEventLoop, subproc->mProcessHandle.Get(), &subproc->mProcessOptions);
    subproc->mProcessHandle.MakeValid();
    if (ret != 0)
    {
        LOG4CXX_ERROR(spLogger, "Error spawning process: " << uv_strerror(ret));
        std::stringstream buf;
        for (const auto& arg : subproc->mCmd)
        {
            buf << arg << " ";
        }
        LOG4CXX_ERROR(spLogger,
                      "Error running process with args: " << buf.str());
        subproc->ProcessExited(100, 0);
    }
}

void EventLoop::RegisterPoller(Poller* poller)
{
    int ret = uv_poll_init(mEventLoop, poller->mHandle->Get(), poller->mFD);
    poller->mHandle->MakeValid();
    assert(ret == 0);
}

void EventLoop::RegisterFileChangeEvent(FileChangeEvent* filechange)
{
    auto ret = uv_fs_event_init(mEventLoop, filechange->mHandle->Get());
    filechange->mHandle->MakeValid();

    assert(ret == 0);
}

struct WorkContext {
    std::shared_ptr<uv_work_t> work_request;
    EventLoop::WorkType work;
    EventLoop::WorkCompleteType work_complete;
    void *user_context;
};

WorkCancellable EventLoop::QueueWork(WorkType work, WorkCompleteType work_complete, void *context)
{
    // uv_work_t isn't a handle and doesn't need to be closed so no need for
    // UvHandle here.
    std::shared_ptr<uv_work_t> work_request(new uv_work_t);
    int queue_result = 0;

    auto work_context = new WorkContext();
    work_request->data = work_context;

    work_context->work_request = work_request;
    work_context->work = std::move(work);
    work_context->work_complete = std::move(work_complete);
    work_context->user_context = context;

    if ((queue_result = uv_queue_work(mEventLoop,
                                      work_request.get(),
                                      &EventLoop::work_callback,
                                      &EventLoop::after_work_callback) < 0))
    {
        LOG4CXX_ERROR(spLogger, "Error queueing work: " << uv_strerror(queue_result));
        delete work_context;
        throw std::runtime_error("failed to queue work");
    }

    return WorkCancellable(work_request);
}

void EventLoop::work_callback(uv_work_t *work)
{
    auto work_context = reinterpret_cast<WorkContext*>(work->data);

    try
    {
        work_context->work(work_context->user_context);
    }
    catch (std::exception&)
    {
    }
}

void EventLoop::after_work_callback(uv_work_t *work, int status)
{
    auto work_context = reinterpret_cast<WorkContext*>(work->data);

    try
    {
        work_context->work_complete(status, work_context->user_context);
    }
    catch (std::exception&)
    {
    }

    delete work_context;
    work->data = nullptr;
}

struct AsyncContext {
    folly::Func fn;
    std::shared_ptr<UvHandle<uv_async_t>> async_handle;
};

void EventLoop::async_callback(uv_async_t *async) {
    auto context = reinterpret_cast<AsyncContext*>(async->data);

    try
    {
        context->fn();
    }
    catch(std::exception&)
    {

    }

    delete context;
    async->data = nullptr;
}

EventLoop::DnsResolver::DnsResolver(
    EventLoop* pEventLoop,
    const std::string& name,
    uint32_t id,
    std::unique_ptr<ITimer> pTimer)
    : mName(name)
    , mPromise()
    , mAddrInfoHandle()
    , mpEventLoop(pEventLoop)
    , mId(id)
    , mpTimer(std::move(pTimer))
    , mTimerId(0)
{
}

void EventLoop::DnsResolver::Cancel()
{
    if (mPromise.isFulfilled())
    {
        return;
    }
    uv_cancel((uv_req_t*)mAddrInfoHandle.Get());
}

void EventLoop::DnsResolver::Resolved(int status, struct addrinfo* res)
{
    mpTimer->Stop(mTimerId);
    mpTimer.reset();
    if (status == 0)
    {
        std::vector<std::string> addrs;
        for (struct addrinfo* current = res; current != nullptr; current = current->ai_next)
        {
            auto addr_in = reinterpret_cast<sockaddr_in*>(current->ai_addr);
            addrs.push_back(ToString(inet_ntoa(addr_in->sin_addr)));
        }
        mPromise.setValue(addrs);
    }
    else
    {
        mPromise.setException(DnsResolutionError(uv_strerror(status)));
    }
}

folly::Future<std::vector<std::string>>
EventLoop::DnsResolver::Start(std::chrono::milliseconds timeout)
{
    struct addrinfo hints = {
        0,
        AF_INET, // I only want IPv4
        SOCK_STREAM, // Plus TCP sockets
        0,
        0,
        0,
        0,
        NULL
    };

    uv_getaddrinfo(
        mpEventLoop->mEventLoop,
        mAddrInfoHandle.Get(),
        on_addrinfo,
        mName.c_str(),
        nullptr,
        &hints
    );

    auto self = shared_from_this();
    mTimerId = mpTimer->Start(timeout.count(), 0, [self] { self->Cancel(); });

    return mPromise.getFuture();
}

void EventLoop::on_addrinfo(uv_getaddrinfo_t* handle, int status, struct addrinfo* res)
{
    DnsResolver* resolver = reinterpret_cast<DnsResolver*>(handle->data);
    resolver->Resolved(status, res);
    uv_freeaddrinfo(res);
}

folly::Future<std::vector<std::string>>
EventLoop::ResolveHostname(
    const std::string& name,
    std::chrono::milliseconds timeout,
    std::unique_ptr<ITimer> pTimer)
{
    mResCounter++;
    if (pTimer == nullptr)
    {
        pTimer = std::make_unique<EventLoopTimer>(this);
    }

    auto res = std::make_shared<DnsResolver>(
        this,
        name,
        mResCounter,
        std::move(pTimer)
    );
    res->mAddrInfoHandle.Get()->data = res.get();
    mResolvers.insert(std::make_pair(mResCounter, res));

    return res->Start(timeout).ensure([this, res]
    {
        CleanupResolver(res->mId);
    });
}

void EventLoop::CleanupResolver(uint32_t id)
{
    mResolvers.erase(id);
}

void EventLoop::add(folly::Func f) {
    std::shared_ptr<UvHandle<uv_async_t>> async_handle(new UvHandle<uv_async_t>());
    int ret = 0;

    auto context = std::make_unique<AsyncContext>();
    context->fn = std::move(f);
    context->async_handle = async_handle;

    async_handle->Get()->data = context.get();

    if ((ret = uv_async_init(mEventLoop,
                             async_handle->Get(),
                             &EventLoop::async_callback)) < 0)
    {
        LOG4CXX_ERROR(spLogger, "failed to initialize async handle: " << uv_strerror(ret));
        return;
    }

    if ((ret = uv_async_send(async_handle->Get())) < 0)
    {
        LOG4CXX_ERROR(spLogger, "failed to send async request: " << uv_strerror(ret));
        return;
    }

    async_handle->MakeValid();
    context.release();
}

}
}
