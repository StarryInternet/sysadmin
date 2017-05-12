#pragma once

#include "decibel/messaging/IRabbitMQMessageHandler.h"
#include "decibel/messaging/Protocol.h"
#include <amqpcpp.h>
#include <boost/circular_buffer.hpp>
#include <boost/signals2/signal.hpp>
#include <folly/futures/Future.h>
#include <google/protobuf/message_lite.h>

namespace decibel
{
namespace messaging
{
class RabbitMQProtocol : public Protocol, public AMQP::ConnectionHandler
{
public:
    using OnConnectedCallbackSignal = boost::signals2::signal<void(RabbitMQProtocol*)>;
    using OnConnectedCallbackFn = OnConnectedCallbackSignal::slot_type;

    using OnErrorCallbackSignal = boost::signals2::signal<void()>;
    using OnErrorCallbackFn = OnErrorCallbackSignal::slot_type;

    using OnDisconnectCallbackSignal = boost::signals2::signal<void()>;
    using OnDisconnectCallbackFn = OnDisconnectCallbackSignal::slot_type;

    using Topic = std::string;

public:
    RabbitMQProtocol();

    virtual ~RabbitMQProtocol();

    virtual void OnConnectionLost(int status);

    virtual void OnDataReceived(const std::vector<char>& data);

    virtual void OnConnectionMade();

    virtual folly::Future<folly::Unit>
    RegisterHandler(const Topic& topic, RabbitMQMessageHandlerPtr pHandler);

    virtual RabbitMQMessageHandlerPtr GetHandler(const Topic& topic);

    virtual void Publish(const Topic& topic,
                         const google::protobuf::MessageLite& message);

    virtual void Publish(const Topic& topic, const std::string& message);

    // AMQP::ConnectionHandler
    virtual void
    onData(AMQP::Connection* pConnection, const char* pData, size_t size);
    virtual void onConnected(AMQP::Connection* pConnection);
    virtual void onError(AMQP::Connection* pConnection, const char* message);
    virtual void onClosed(AMQP::Connection* pConnection);
    virtual uint16_t onNegotiate(AMQP::Connection* pConnection, uint16_t interval);

    virtual void RegisterOnConnected(OnConnectedCallbackFn onConnectedCallback);

    virtual void RegisterOnError(OnErrorCallbackFn onErrorCallback);

    virtual void
    RegisterOnDisconnect(OnDisconnectCallbackFn onDisconnectCallback);

    virtual folly::Future<folly::Unit> Shutdown();

private:
    void DoRead(const std::vector<char>& data);

    void DoWrite(const char* pData, size_t size);

    void DoOnError(const char* message);

    void DoOnConnected(RabbitMQProtocol* pProtocol);

    void DoOnDisconnect(int);

    folly::Future<folly::Unit> ConsumeTopic(const Topic& topic,
                                            RabbitMQMessageHandlerPtr pHandler);

private:
    std::unique_ptr<AMQP::Connection> mpConnection;
    std::unique_ptr<AMQP::Channel> mpChannel;
    std::unordered_map<std::string, RabbitMQMessageHandlerPtr> mHandlers;
    boost::circular_buffer<char> mBuffer;
    OnErrorCallbackSignal mOnErrorCallback;
    OnConnectedCallbackSignal mOnConnectedCallback;
    OnDisconnectCallbackSignal mOnDisconnectCallback;
    std::vector<std::string> mQueues;
};

} // messaging
} // decibel
