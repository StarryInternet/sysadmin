#pragma once

#include <cstdint>
#include <memory>

#include <folly/futures/Future.h>

namespace decibel
{
namespace messaging
{
class ITransport
{
public:
    virtual ~ITransport();

    virtual void Write(const char* data, size_t len) = 0;

    virtual void Disconnect() = 0;

    virtual folly::Future<folly::Unit> Reconnect() = 0;

    void OnDisconnect(std::function<void(int)> callback);

protected:
    std::function<void(int)> mOnDisconnectCallback;
};

typedef std::unique_ptr<ITransport> TransportPtr;

} // messaging
} // decibel
