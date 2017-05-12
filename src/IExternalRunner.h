
#pragma once

#include <folly/futures/Future.h>

#include <vector>
#include <string>

class IExternalRunner
{
public:
    typedef std::vector<std::string> FormattedValues;

public:
    virtual folly::Future<folly::Unit> Run(const FormattedValues& values) const = 0;
};
