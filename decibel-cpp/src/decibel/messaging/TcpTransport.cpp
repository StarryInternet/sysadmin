#include "decibel/messaging/TcpTransport.h"

#include "decibel/niceuv/TcpConn.h"

#include <folly/futures/Future.h>
#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("TcpTransport"));
}

namespace decibel
{
namespace messaging
{
TcpTransport::TcpTransport(std::unique_ptr<niceuv::TcpConn> pTcpConn,
                           const std::string& host,
                           uint16_t port)
    : ITransport(), mpConnection(std::move(pTcpConn)), mHost(host), mPort(port)
{
    mpConnection->RegisterDisconnectCallback([this](int) { Disconnect(); });
}

TcpTransport::~TcpTransport()
{
    mpConnection->ClearCallbacks();
}

void TcpTransport::Write(const char* data, size_t len)
{
    mpConnection->Write(data, len);
}

void TcpTransport::Disconnect()
{
    mpConnection->Stop();
    if (mOnDisconnectCallback)
    {
        mOnDisconnectCallback(0);
    }
}

folly::Future<folly::Unit> TcpTransport::Reconnect()
{
    auto connectPromise = std::make_shared<folly::Promise<folly::Unit>>();
    mpConnection->RegisterConnectCallback(
        [connectPromise](int) { connectPromise->setValue(); });
    mpConnection->RegisterConnectErrorCallback([connectPromise](int) {
        connectPromise->setException(
            std::runtime_error("Couldn't reconnect :("));
    });
    mpConnection->Reset();
    mpConnection->Connect(mHost, mPort);
    mpConnection->Start();
    return connectPromise->getFuture();
}

} // messaging
} // decibel
