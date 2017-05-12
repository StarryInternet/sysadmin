#include "decibel/messaging/RabbitMQProtocol.h"

#include "decibel/messaging/Exchanges.h"

#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("RabbitMQProtocol"));
}

using namespace std::placeholders;

namespace decibel
{
namespace messaging
{
RabbitMQProtocol::RabbitMQProtocol()
    : Protocol()
    , mpConnection()
    , mpChannel()
    , mHandlers()
    , mBuffer(4096)
    , mOnErrorCallback()
    , mOnConnectedCallback()
    , mOnDisconnectCallback()
    , mQueues()
{
}

RabbitMQProtocol::~RabbitMQProtocol()
{
}

folly::Future<folly::Unit>
RabbitMQProtocol::RegisterHandler(const Topic& topic,
                                  RabbitMQMessageHandlerPtr pHandler)
{
    if (mHandlers.find(topic) != mHandlers.end())
    {
        return folly::makeFuture();
    }
    mHandlers.emplace(topic, pHandler);
    return ConsumeTopic(topic, pHandler);
}

folly::Future<folly::Unit>
RabbitMQProtocol::ConsumeTopic(const Topic& topic,
                               RabbitMQMessageHandlerPtr pHandler)
{
    auto error_handler = [topic](const std::string& msg) {
        LOG4CXX_ERROR(spLogger,
                      "Failed to register handler for " << topic << ": "
                                                        << msg);
    };

    // Wrap AMQP calls in futures.
    auto declareQueuePromise = std::make_shared<folly::Promise<std::string>>();
    auto bindQueuePromise = std::make_shared<folly::Promise<std::string>>();
    auto consumePromise = std::make_shared<folly::Promise<folly::Unit>>();

    auto consume = [=](const std::string& queueName) {
        mpChannel->consume(queueName, AMQP::noack)
            .onReceived(std::bind(&IRabbitMQMessageHandler::HandleData,
                                  pHandler,
                                  std::placeholders::_1,
                                  std::placeholders::_2,
                                  std::placeholders::_3))
            .onError(std::bind(error_handler, "Failed to 'consume'"))
            .onSuccess([consumePromise]() { consumePromise->setValue(); });
        return consumePromise->getFuture();
    };

    auto bind_queue = [=](const std::string& queueName) {
        mpChannel->bindQueue(Exchanges::DECIBEL, queueName, topic)
            .onError(std::bind(error_handler, "Failed to bind queue"))
            .onSuccess([bindQueuePromise, queueName]() {
                bindQueuePromise->setValue(queueName);
            });
        return bindQueuePromise->getFuture();
    };

    mpChannel->declareQueue()
        .onSuccess([this, declareQueuePromise](
            const std::string& queueName, uint32_t, uint32_t) {
            mQueues.push_back(queueName);
            declareQueuePromise->setValue(queueName);
        })
        .onError(std::bind(error_handler, "Failed to declare queue"));

    auto declareQueueFuture = declareQueuePromise->getFuture();

    return declareQueueFuture.then(bind_queue).then(consume);
}

RabbitMQMessageHandlerPtr RabbitMQProtocol::GetHandler(const std::string& topic)
{
    return mHandlers.at(topic);
}

void RabbitMQProtocol::Publish(const std::string& topic,
                               const google::protobuf::MessageLite& message)
{
    std::string output;
    if (!message.IsInitialized())
    {
        throw std::runtime_error("Message is not initialized");
    }
    message.SerializeToString(&output);
    Publish(topic, output);
}

void RabbitMQProtocol::Publish(const std::string& topic,
                               const std::string& message)
{
    if (!mpChannel)
    {
        LOG4CXX_WARN(spLogger,
                     "Attempting to publish to "
                         << topic
                         << " while disconnected from RabbitMQ");
        return;
    }
    mpChannel->publish(Exchanges::DECIBEL, topic, message);
}

void RabbitMQProtocol::OnConnectionLost(int)
{
    mQueues.clear();
    mpConnection.reset();
    mpChannel.reset();
}

void RabbitMQProtocol::OnDataReceived(const std::vector<char>& data)
{
    DoRead(data);
}

void RabbitMQProtocol::OnConnectionMade()
{
    mpConnection.reset(new AMQP::Connection(this));
    mpChannel.reset(new AMQP::Channel(mpConnection.get()));
    mpChannel->onError(
        std::bind(&RabbitMQProtocol::DoOnError, this, std::placeholders::_1));
    mpChannel->onReady(std::bind(&RabbitMQProtocol::DoOnConnected, this, this));
    mpChannel->declareExchange(Exchanges::DECIBEL, AMQP::topic);
}

void RabbitMQProtocol::onData(AMQP::Connection*, const char* data, size_t size)
{
    DoWrite(data, size);
}

void RabbitMQProtocol::onConnected(AMQP::Connection*)
{
    LOG4CXX_INFO(spLogger, "AMQP connection opened");
    for (auto& topic_handler : mHandlers)
    {
        ConsumeTopic(topic_handler.first, topic_handler.second);
    }
}

void RabbitMQProtocol::onError(AMQP::Connection*, const char* message)
{
    DoOnError(message);
}

void RabbitMQProtocol::onClosed(AMQP::Connection*)
{
    LOG4CXX_INFO(spLogger, "AMQP connection closed");
    mpChannel.reset();
    mpConnection.reset();
    DoOnDisconnect(0);
}

uint16_t RabbitMQProtocol::onNegotiate(AMQP::Connection* , uint16_t interval)
{
    return interval;
}

void RabbitMQProtocol::DoWrite(const char* pData, size_t size)
{
    mpTransport->Write(pData, size);
}

void RabbitMQProtocol::DoRead(const std::vector<char>& data)
{
    if (data.empty() || !mpConnection)
    {
        return;
    }
    auto newSize = data.size() + mBuffer.size();
    if (newSize > mBuffer.capacity())
    {
        mBuffer.set_capacity(newSize);
    }
    mBuffer.insert(mBuffer.end(), data.begin(), data.end());
    std::vector<char> to_process(mBuffer.begin(), mBuffer.end());
    auto processed_byte_count =
        mpConnection->parse(to_process.data(), to_process.size());
    mBuffer.erase(mBuffer.begin(), mBuffer.begin() + processed_byte_count);
}

void RabbitMQProtocol::RegisterOnError(OnErrorCallbackFn onErrorCallback)
{
    mOnErrorCallback.connect(onErrorCallback);
}

void RabbitMQProtocol::RegisterOnConnected(
    OnConnectedCallbackFn onConnectedCallback)
{
    mOnConnectedCallback.connect(onConnectedCallback);
}

void RabbitMQProtocol::RegisterOnDisconnect(
    OnDisconnectCallbackFn onDisconnectCallback)
{
    mOnDisconnectCallback.connect(onDisconnectCallback);
}

void RabbitMQProtocol::DoOnError(const char* message)
{
    LOG4CXX_ERROR(spLogger, "Channel Error: " << message);
    mOnErrorCallback();
}

void RabbitMQProtocol::DoOnConnected(RabbitMQProtocol* pProtocol)
{
    mOnConnectedCallback(pProtocol);
}

void RabbitMQProtocol::DoOnDisconnect(int)
{
    LOG4CXX_INFO(spLogger, "Disconnected from RabbitMQ");
    mOnDisconnectCallback();
}

folly::Future<folly::Unit> RabbitMQProtocol::Shutdown()
{
    std::vector<folly::Future<folly::Unit>> futures;
    for (auto& queue : mQueues)
    {
        auto pPromise = std::make_shared<folly::Promise<folly::Unit>>();
        mpChannel->cancel(queue);
        mpChannel->removeQueue(queue).onSuccess(
            [pPromise]() { pPromise->setValue(); });
        futures.push_back(pPromise->getFuture());
    }
    return folly::collectAll(futures).unit();
}

} // messaging
} // decibel
