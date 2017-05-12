#pragma once

#include "decibel/messaging/ITransport.h"
#include "decibel/niceuv/TcpConn.h"

namespace decibel
{
namespace messaging
{
class TcpTransport : public ITransport
{
public:
    TcpTransport(std::unique_ptr<niceuv::TcpConn> pConnection,
                 const std::string& host,
                 uint16_t port);

    ~TcpTransport();

    virtual void Write(const char* data, size_t len);

    virtual void Disconnect();

    virtual folly::Future<folly::Unit> Reconnect();

private:
    std::unique_ptr<niceuv::TcpConn> mpConnection;
    std::string mHost;
    uint16_t mPort;
};

} // messaging
} // decibel
