
#pragma once

#include <folly/futures/Future.h>

#include <vector>
#include <string>

class IExternalRunner
{
public:
    typedef std::vector<std::string> Arguments;

public:
    virtual folly::Future<folly::Unit> Run(const Arguments& values) const = 0;
};
