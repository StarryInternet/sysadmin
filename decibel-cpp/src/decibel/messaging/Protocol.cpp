#include "decibel/messaging/Protocol.h"

#include "decibel/messaging/Reactor.h"

#include <log4cxx/logger.h>
#include <memory>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("Protocol"));
}

using namespace std::placeholders;

namespace decibel
{
namespace messaging
{
Protocol::Protocol()
    : mpTransport()
    , mOnConnectionLostCallback([](int) { return folly::makeFuture(false); })
    , mOnConnectionDeathCallback([](){})
    , mId(reinterpret_cast<Protocol::Id>(this))
{
}

Protocol::~Protocol()
{
}

void Protocol::MakeConnection(TransportPtr pTransport)
{
    mpTransport = std::move(pTransport);
    mpTransport->OnDisconnect(
        std::bind(&Protocol::InternalConnectionLost, this, _1));
    OnConnectionMade();
}

void Protocol::RegisterOnConnectionLostCallback(const OnConnectionLostFn& cb)
{
    mOnConnectionLostCallback = cb;
}

void Protocol::RegisterOnConnectionDeathCallback(const OnConnectionDeathFn& cb)
{
    mOnConnectionDeathCallback = cb;
}

void Protocol::InternalConnectionLost(int status)
{
    OnConnectionLost(status);
    Reconnect(status);
}

void Protocol::Reconnect(int status)
{
    mOnConnectionLostCallback(status).thenValue([this](bool reconnect) {
        if (reconnect)
        {
            LOG4CXX_INFO(spLogger, "Reconnecting...");
            mpTransport->Reconnect().thenValue([this](auto /*unused*/) {
                LOG4CXX_INFO(spLogger, "Successfully reconnected");
                this->MakeConnection(std::move(mpTransport));
            });
        }
        else
        {
            LOG4CXX_INFO(spLogger, "Allowing connection to die...");
            mOnConnectionDeathCallback();
        }
    });
}

void Protocol::OnConnectionLost(int)
{
}

void Protocol::OnConnectionMade()
{
}

void Protocol::OnDataReceived(const std::vector<char>&)
{
}

folly::Future<folly::Unit> Protocol::Shutdown()
{
    return folly::makeFuture();
}

Protocol::OnConnectionLostFn MakeReconnectStrategy(Reactor* pReactor,
                                                   size_t timeoutMs)
{
    return [pReactor, timeoutMs](int) {
        auto pPromise = std::make_shared<folly::Promise<bool>>();
        pReactor->CallLater(timeoutMs,
                            [pPromise]() { pPromise->setValue(true); });
        return pPromise->getFuture();
    };
}

} // messaging
} // decibel
