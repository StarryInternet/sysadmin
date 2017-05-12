
#pragma once

#include <string>

namespace decibel
{
namespace messaging
{
typedef std::string Exchange;

struct Exchanges
{
    static const Exchange DECIBEL;

private:
    Exchanges() = delete;
};

} // messaging
} // decibel
