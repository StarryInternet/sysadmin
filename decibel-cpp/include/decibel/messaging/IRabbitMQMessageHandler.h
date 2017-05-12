
#pragma once

#include <amqpcpp.h>
#include <memory>

namespace decibel
{
namespace messaging
{
class IRabbitMQMessageHandler
{
public:
    virtual ~IRabbitMQMessageHandler();

    virtual void HandleData(AMQP::Message&& message,
                            uint64_t deliveryTag,
                            bool redelivered) = 0;
};

typedef std::shared_ptr<IRabbitMQMessageHandler> RabbitMQMessageHandlerPtr;

} // messaging
} // decibel
