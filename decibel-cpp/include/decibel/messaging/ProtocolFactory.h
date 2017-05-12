
#pragma once

#include "decibel/messaging/IProtocolFactory.h"
#include "decibel/messaging/Protocol.h"
#include "decibel/stdlib.h"

namespace decibel
{
namespace messaging
{
template <class P>
class ProtocolFactory : public IProtocolFactory
{
public:
    ProtocolFactory() : IProtocolFactory()
    {
    }

    virtual ~ProtocolFactory()
    {
    }

    virtual std::unique_ptr<Protocol> Construct()
    {
        return std::make_unique<P>();
    }
};
}
}
