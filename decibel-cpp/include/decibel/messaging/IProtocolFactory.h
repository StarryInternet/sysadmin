
#pragma once

#include "decibel/messaging/Protocol.h"

namespace decibel
{
namespace messaging
{
class IProtocolFactory
{
public:
    IProtocolFactory()
    {
    }

    virtual ~IProtocolFactory()
    {
    }

    virtual std::unique_ptr<Protocol> Construct() = 0;
};
}
}
