#pragma once

#include <folly/futures/Future.h>
#include <functional>

#include "decibel/messaging/Reactor.h"

namespace decibel
{
namespace messaging
{
class LoopingTask
{
public:
    typedef std::function<void()> TaskFn;

public:
    LoopingTask(Reactor* pReactor, TaskFn&& fn);

    ~LoopingTask();

    /** Future will be fulfilled when the LoopingTask is `Stop`ped or the
     * underlying function
     * call throws an error. In the error case, the error can be handled through
     * the onError
     * method of the future returned by `Start`.
     */
    folly::Future<folly::Unit> Start(int interval);

    void Reschedule(int interval);

    void Stop();

    bool Running() const;

private:
    void ScheduleNext(int timeout);

    void ScheduledFn();

    void OnCall();

    void OnError(const std::exception& e);

private:
    Reactor* mpReactor;
    TaskFn mFn;
    bool mRunning;
    std::shared_ptr<folly::Promise<folly::Unit>> mPromise;
    int mInterval;
};

} // messaging
} // decibel
