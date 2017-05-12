#pragma once

#include <boost/optional.hpp>
#include <functional>

#include "ProtobufProtocol.h"
#include "decibel/Protobuf.h"

namespace decibel
{
namespace messaging
{
template <
    typename MessageType,
    typename ReturnType,
    typename LengthT
>
class ProtobufOneofProtocol : public ProtobufProtocol<MessageType, LengthT>
{
public:
    typedef OneofDispatcher<MessageType,
                            boost::optional<ReturnType>,
                            Protocol::Id>
        DispatcherT;

    using SendErrorCb = std::function<
        void(
            ProtobufOneofProtocol<MessageType, ReturnType, LengthT>*,
            const ReturnType&
        )
    >;

public:
    ProtobufOneofProtocol(DispatcherT dispatcher)
        : ProtobufProtocol<MessageType, LengthT>()
        , mDispatcher(dispatcher)
        , mSendErrorCallback(boost::none)
        , mValidThis(std::make_shared<bool>(true))
    {
    }

    virtual ~ProtobufOneofProtocol()
    {
        *mValidThis = false;
    }

    virtual void RegisterSendErrorCallback(SendErrorCb onErrCb)
    {
        mSendErrorCallback = onErrCb;
    }

    virtual void HandleMessage(MessageType&& protobufMessage)
    {
        auto ret = mDispatcher.Dispatch(
            std::move(protobufMessage), "payload", this->mId);
        auto valid_ptr = mValidThis;
        ret.then([this, valid_ptr](boost::optional<ReturnType> maybeMessage) {
            if (maybeMessage && *valid_ptr)
            {
                try
                {
                    this->Send(*maybeMessage);
                }
                catch (const MessageSendError& err)
                {
                    if (mSendErrorCallback)
                    {
                        (*mSendErrorCallback)(this, *maybeMessage);
                    }
                }
            }
        });
    }

private:
    DispatcherT mDispatcher;
    boost::optional<SendErrorCb> mSendErrorCallback;
    std::shared_ptr<bool> mValidThis;
};
}
}
