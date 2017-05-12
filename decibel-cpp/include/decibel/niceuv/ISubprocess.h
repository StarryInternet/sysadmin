
#pragma once

#include "SubprocessHandler.h"

#include <folly/futures/Future.h>

namespace decibel
{
namespace niceuv
{
class ISubprocess
{
public:
    virtual folly::Future<SubprocessHandler::Buffer> RunSubprocess() = 0;

    virtual const std::vector<std::string>& GetCmd() const = 0;
};
}
}
