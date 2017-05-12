
#pragma once

#include <memory>
#include <string>

#include "FramedBuffer.h"
#include "Protocol.h"
#include "decibel/Protobuf.h"

namespace decibel
{
namespace messaging
{

class MessageSendError : public std::runtime_error
{
public:
    MessageSendError(const std::string& str) : std::runtime_error(str)
    {
    }
};

class MessageSizeError : public MessageSendError
{
public:
    MessageSizeError(const std::string& str) : MessageSendError(str)
    {
    }
};

template <
    typename MessageType,
    typename LengthT
>
class ProtobufProtocol : public Protocol
{
private:
    using BufferType = FramedBuffer<LengthT>;
    using BufferLengthType = typename BufferType::LengthType;

public:
    ProtobufProtocol() : mBuffer()
    {
    }

    virtual ~ProtobufProtocol()
    {
    }

    // This method and class will have to do the buffering of protobuf data when
    // we implement
    // the streaming protobuf messages business
    virtual void OnDataReceived(const std::vector<char>& data)
    {
        BufferLengthType bytesUsed = 0;
        while (bytesUsed < data.size())
        {
            bytesUsed += mBuffer.AddData(data.data() + bytesUsed,
                                         data.size() - bytesUsed);
            if (mBuffer.Ready())
            {
                MessageType protobufMessage;
                protobufMessage.ParseFromArray(mBuffer.Data(), mBuffer.Size());
                HandleMessage(std::move(protobufMessage));
                mBuffer = BufferType();
            }
        }
    }

    virtual void Send(const google::protobuf::MessageLite& message)
    {
        std::string output;
        message.SerializeToString(&output);
        if (output.size() > std::numeric_limits<BufferLengthType>::max())
        {
            throw MessageSizeError("Message too large to transfer");
        }
        else
        {
            BufferLengthType length = output.size();
            char sizeBuffer[sizeof(BufferLengthType)];
            memcpy(sizeBuffer, &length, sizeof(BufferLengthType));
            mpTransport->Write(sizeBuffer, sizeof(BufferLengthType));
            mpTransport->Write(output.c_str(), length);
        }
    }

    virtual void HandleMessage(MessageType&& protobufMessage) = 0;

private:
    FramedBuffer<LengthT> mBuffer;
};
}
}
