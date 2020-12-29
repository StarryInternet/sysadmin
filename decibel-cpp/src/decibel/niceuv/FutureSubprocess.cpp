// Copyright 2017, 2020. Starry, Inc. All Rights Reserved.
#include <algorithm>

#include <log4cxx/logger.h>
#include <uv.h>

#include "decibel/niceuv/FutureSubprocess.h"

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("FutureSubprocess"));
}

namespace decibel
{
namespace niceuv
{
FutureSubprocess::FutureSubprocess(EventLoop* loop,
                                   const std::vector<std::string>& cmd,
                                   const SubprocessHandler::SubprocessOptions& options)
    : mSubprocess(loop, cmd, options)
    , mFullyDetach(false)
{
    mFullyDetach = std::find(options.begin(),
                             options.end(),
                             SubprocessHandler::SubprocessOption::STARRY_PROCESS_FULLY_DETACH) != options.end() ? true : false;
}

folly::Future<SubprocessHandler::Buffer> FutureSubprocess::RunSubprocess()
{
    auto pStdOutPromise =
        std::make_shared<folly::Promise<SubprocessHandler::Buffer>>();
    auto pStdErrPromise =
        std::make_shared<folly::Promise<SubprocessHandler::Buffer>>();
    auto pExitPromise = std::make_shared<folly::Promise<int>>();

    auto stdoutcallback =
        [pStdOutPromise](const SubprocessHandler::Buffer& buffer) {
            pStdOutPromise->setValue(buffer);
        };
    auto stderrcallback =
        [pStdErrPromise](const SubprocessHandler::Buffer& buffer) {
            pStdErrPromise->setValue(buffer);
        };
    auto exitcallback = [pExitPromise](int status) {
        pExitPromise->setValue(status);
    };

    if (mFullyDetach)
    {
        pStdOutPromise->setValue(SubprocessHandler::Buffer());
        pStdErrPromise->setValue(SubprocessHandler::Buffer());
        pExitPromise->setValue(0);
    }
    else
    {
        mSubprocess.RegisterStdOutCallback(stdoutcallback, false);
        mSubprocess.RegisterStdErrCallback(stderrcallback);
        mSubprocess.RegisterProcessExitCallback(exitcallback);
    }

    auto pWrappedPromise =
        std::make_shared<folly::Promise<SubprocessHandler::Buffer>>();
    folly::collectAll(pStdOutPromise->getFuture(),
                      pExitPromise->getFuture(),
                      pStdErrPromise->getFuture())
        .toUnsafeFuture()
        .thenValue([pWrappedPromise](
            const std::tuple<folly::Try<SubprocessHandler::Buffer>,
                             folly::Try<int>,
                             folly::Try<SubprocessHandler::Buffer>>& t) {
            auto buffer = std::get<0>(t).value();
            auto status = std::get<1>(t).value();
            auto errBuffer = std::get<2>(t).value();
            if (status != 0)
            {
                pWrappedPromise->setException(FutureSubprocessError(
                    uv_strerror(status), buffer, errBuffer));
            }
            else
            {
                pWrappedPromise->setValue(buffer);
            }
        });
    mSubprocess.RunProcess();
    return pWrappedPromise->getFuture();
}

const std::vector<std::string>& FutureSubprocess::GetCmd() const
{
    return mSubprocess.GetCmd();
}
}
}
