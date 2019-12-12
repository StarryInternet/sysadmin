#include "decibel/messaging/LoopingTask.h"

#include <boost/assert.hpp>

namespace decibel
{
namespace messaging
{
LoopingTask::LoopingTask(Reactor* pReactor, TaskFn&& fn)
    : mpReactor(pReactor)
    , mFn(std::move(fn))
    , mRunning(false)
    , mPromise(new folly::Promise<folly::Unit>)
    , mInterval(0)
{
    BOOST_ASSERT(mFn);
}

LoopingTask::~LoopingTask()
{
}

/** Future will be fulfilled when the LoopingTask is `Stop`ped or the underlying
 * function
 * call throws an error. In the error case, the error can be handled through the
 * onError
 * method of the future returned by `Start`.
 */
folly::Future<folly::Unit> LoopingTask::Start(int interval)
{
    BOOST_ASSERT_MSG(
        !Running(),
        "Attempting to start a LoopingTask that is already running");
    mInterval = interval;
    mRunning = true;
    ScheduleNext(interval);
    return mPromise->getFuture();
}

void LoopingTask::Reschedule(int interval)
{
    BOOST_ASSERT(Running());
    mInterval = interval;
}

void LoopingTask::Stop()
{
    if (!Running())
    {
        return;
    }
    mRunning = false;
    mPromise->setValue(folly::Unit{});
    mPromise.reset(new folly::Promise<folly::Unit>);
}

bool LoopingTask::Running() const
{
    return mRunning;
}

void LoopingTask::ScheduleNext(int timeout)
{
    mpReactor->CallLater(timeout, std::bind(&LoopingTask::ScheduledFn, this));
}

void LoopingTask::ScheduledFn()
{
    if (!this->Running())
    {
        return;
    }
    // This will call mFn and fulfill a future with the result / thrown
    // exception
    folly::makeFutureWith(this->mFn)
        .thenValue(std::bind(&LoopingTask::OnCall, this))
        .thenError(folly::tag_t<std::exception>{}, [this](const auto& e) { this->OnError(e); });
}

void LoopingTask::OnCall()
{
    if (this->Running())
    {
        this->ScheduleNext(this->mInterval);
    }
}

void LoopingTask::OnError(const std::exception& e)
{
    if (this->Running())
    {
        this->mPromise->setException(e);
        mRunning = false;
    }
}

} // messaging
} // decibel
