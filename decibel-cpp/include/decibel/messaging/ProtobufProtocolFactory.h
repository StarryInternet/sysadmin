
#pragma once

#include <boost/optional.hpp>
#include <memory>

#include "IProtocolFactory.h"
#include "ProtobufOneofProtocol.h"
#include "ProtobufProtocol.h"
#include "decibel/Protobuf.h"
#include "decibel/stdlib.h"

namespace decibel
{
namespace messaging
{
template <class MessageType, class ReturnType, class LengthT>
class ProtobufProtocolFactory : public IProtocolFactory
{
public:
    typedef ProtobufOneofProtocol<MessageType, ReturnType, LengthT> ProtocolT;
    typedef typename ProtocolT::DispatcherT DispatcherT;

private:
    using SendErrorCb = typename ProtobufOneofProtocol<
        MessageType,
        ReturnType,
        LengthT
    >::SendErrorCb;

public:
    ProtobufProtocolFactory(DispatcherT dispatcher)
        : mDispatcher(dispatcher)
        , mSendErrorCallback(boost::none)
    {
    }

    virtual ~ProtobufProtocolFactory()
    {
    }

    virtual void
        RegisterSendErrorCallback(SendErrorCb onErrCb)
    {
        mSendErrorCallback = onErrCb;
    }

    virtual std::unique_ptr<Protocol> Construct()
    {
        if (mSendErrorCallback)
        {
            auto protocol = std::make_unique<ProtocolT>(mDispatcher);
            protocol->RegisterSendErrorCallback(*mSendErrorCallback);
            return protocol;
        }
        else
        {
            return std::make_unique<ProtocolT>(mDispatcher);
        }
    }

private:
    DispatcherT mDispatcher;
    boost::optional<SendErrorCb> mSendErrorCallback;
};
}
}
