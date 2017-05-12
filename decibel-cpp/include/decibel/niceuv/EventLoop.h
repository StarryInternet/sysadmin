
#pragma once

#include <functional>

#include <uv.h>

#include <folly/Executor.h>
#include <folly/futures/Future.h>

#define BOOST_BIND_NO_PLACEHOLDERS

#include "decibel/niceuv/BaseTimerEvent.h"
#include "decibel/niceuv/ITimer.h"
#include "decibel/niceuv/IdleEvent.h"
#include "decibel/niceuv/OneShotTimerEvent.h"
#include "decibel/niceuv/SubprocessHandler.h"
#include "decibel/niceuv/TcpConn.h"
#include "decibel/niceuv/Poller.h"
#include "decibel/niceuv/WorkCancellable.h"
#include "decibel/niceuv/FileChangeEvent.h"
#include "decibel/ToString.h"

namespace decibel
{
namespace niceuv
{
extern const char* strerror(int status);

class DnsResolutionError : public std::runtime_error
{
public:
    DnsResolutionError(const std::string& str)
        : std::runtime_error(str)
    {
    }
};

class EventLoop: public folly::Executor
{
public:
    typedef std::function<void(void*)> WorkType;
    typedef std::function<void(int,void*)> WorkCompleteType;
    typedef std::function<void(int status, const std::vector<std::string>&)> ResolvedCallback;

public:
    EventLoop();

    ~EventLoop();

    void RunForever();

    int LoopOnce();

    void Stop();

    // I could hear an argument that this should be through an interface that
    // hides access to the run methods above

    // Event type registration functions
    void RegisterIdleEvent(IdleEvent* idler);

    void RegisterTcpConn(TcpConn* conn);

    void RegisterTimerEvent(EventLoopTimer::HandleWrapper* timer);

    void RegisterAndSpawnSubprocess(SubprocessHandler* subproc);

    void RegisterPoller(Poller* poll);

    void RegisterFileChangeEvent(FileChangeEvent* filechange);

    WorkCancellable
    QueueWork(WorkType work, WorkCompleteType work_complete, void *context = nullptr);

    virtual void add(folly::Func f);

    folly::Future<std::vector<std::string>>
    ResolveHostname(
        const std::string& name,
        std::chrono::milliseconds timeout = std::chrono::milliseconds(10000),
        std::unique_ptr<ITimer> = nullptr
    );

private:

    struct DnsResolver : std::enable_shared_from_this<DnsResolver>
    {
        DnsResolver(
            EventLoop* pEventLoop,
            const std::string& name,
            uint32_t id,
            std::unique_ptr<ITimer> pTimer
        );

        folly::Future<std::vector<std::string>>
        Start(std::chrono::milliseconds timeout);

        void Cancel();

        void Resolved(int status, struct addrinfo* res);

        const std::string mName;
        folly::Promise<std::vector<std::string>> mPromise;
        UvHandle<uv_getaddrinfo_t> mAddrInfoHandle;
        EventLoop* mpEventLoop;
        uint32_t mId;
        std::unique_ptr<ITimer> mpTimer;
        ITimer::TimerId mTimerId;
    };

    void CleanupResolver(uint32_t id);

private:
    static void work_callback(uv_work_t *work);
    static void after_work_callback(uv_work_t *work, int status);
    static void async_callback(uv_async_t *async);

    static void on_addrinfo(uv_getaddrinfo_t* handle, int status, struct addrinfo* res);

private:
    uv_loop_t* mEventLoop;
    std::unordered_map<uint32_t, std::shared_ptr<DnsResolver>> mResolvers;
    uint32_t mResCounter;
};
}
}
