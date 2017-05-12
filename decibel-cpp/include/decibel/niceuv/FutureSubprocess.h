
#pragma once

#include "EventLoop.h"
#include "ISubprocess.h"
#include "SubprocessHandler.h"

#include <folly/futures/Future.h>
#include <stdexcept>

namespace decibel
{
namespace niceuv
{
class FutureSubprocessError : public std::runtime_error
{
public:
    FutureSubprocessError(const std::string& str,
                          const SubprocessHandler::Buffer& buffer,
                          const SubprocessHandler::Buffer& errBuffer)
        : std::runtime_error(str), mBuffer(buffer), mErrBuffer(errBuffer)
    {
    }

    SubprocessHandler::Buffer mBuffer;
    SubprocessHandler::Buffer mErrBuffer;
};

class FutureSubprocess : public ISubprocess
{
public:
    FutureSubprocess(EventLoop* loop,
                     const std::vector<std::string>& cmd,
                     const SubprocessHandler::SubprocessOptions& options = {});

public: // ISubprocess
    virtual folly::Future<SubprocessHandler::Buffer> RunSubprocess();

    virtual const std::vector<std::string>& GetCmd() const;

private:
    SubprocessHandler mSubprocess;
    bool mFullyDetach;
};
}
}
