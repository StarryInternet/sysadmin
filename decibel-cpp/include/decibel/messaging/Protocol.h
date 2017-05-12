#pragma once

#include "ITransport.h"

#include <folly/futures/Future.h>
#include <memory>
#include <vector>

namespace decibel
{
namespace messaging
{
class Reactor;

class Protocol
{
public:
    typedef std::function<folly::Future<bool>(int)> OnConnectionLostFn;
    typedef std::function<void()> OnConnectionDeathFn;
    typedef size_t Id;

public:
    Protocol();

    virtual ~Protocol();

    void MakeConnection(TransportPtr pTransport);

    virtual void OnConnectionLost(int status);

    void RegisterOnConnectionLostCallback(const OnConnectionLostFn& cb);

    void RegisterOnConnectionDeathCallback(const OnConnectionDeathFn& cb);

    virtual void OnDataReceived(const std::vector<char>& data);

    virtual void OnConnectionMade();

    virtual folly::Future<folly::Unit> Shutdown();

    void InternalConnectionLost(int status);

private:
    void Reconnect(int status);

protected:
    TransportPtr mpTransport;
    OnConnectionLostFn mOnConnectionLostCallback;
    OnConnectionDeathFn mOnConnectionDeathCallback;
    const Id mId;
};

Protocol::OnConnectionLostFn MakeReconnectStrategy(Reactor* pReactor,
                                                   size_t timeoutMs);

typedef std::unique_ptr<Protocol> ProtocolPtr;

} // messaging
} // decibel
