
#pragma once

#include "decibel/messaging/IRabbitMQMessageHandler.h"

namespace decibel
{
namespace messaging
{
template <typename MessageType>
class RabbitMQMessageHandler : public IRabbitMQMessageHandler
{
public:
    virtual ~RabbitMQMessageHandler()
    {
    }

    virtual void HandleData(AMQP::Message&& message, uint64_t, bool)
    {
        MessageType protobufMessage;
        protobufMessage.ParseFromString(std::string(message.body()));
        HandleMessage(std::move(protobufMessage));
    }

    virtual void HandleMessage(MessageType&& message) = 0;
};

} // messaging
} // decibel
