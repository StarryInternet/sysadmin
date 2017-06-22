
#include "decibel/messaging/TcpServer.h"
#include "decibel/messaging/TcpTransport.h"
#include "decibel/niceuv/TcpConn.h"
#include "decibel/stdlib.h"

#include <log4cxx/logger.h>
#include <uv.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("TcpServer"));
}

namespace decibel
{
namespace messaging
{
TcpServer::TcpServer(niceuv::EventLoop* loop,
                     const std::string& host,
                     uint16_t port,
                     IProtocolFactory* factory)
    : mServer(new niceuv::TcpServer(loop)), mProtocols()
{
    auto connectCb = [factory, this, host, port](niceuv::TcpConnPtr conn) {
        auto pProtocol = factory->Construct();
        auto rawpConn = conn.get();
        auto pTransport =
            std::make_unique<TcpTransport>(std::move(conn), host, port);
        pProtocol->MakeConnection(std::move(pTransport));
        rawpConn->RegisterReadCallback(std::bind(
            &Protocol::OnDataReceived, pProtocol.get(), std::placeholders::_1));
        auto protoiter = mProtocols.insert(std::move(pProtocol)).first;
        auto deleter = [protoiter, this](int status) {
            (*protoiter)->OnConnectionLost(status);
            mProtocols.erase(protoiter);
        };
        rawpConn->RegisterDisconnectCallback(deleter);
        rawpConn->Start();
    };
    mServer->RegisterFatalCallback([](int status) {
        LOG4CXX_ERROR(spLogger,
                      "Fatal error while listening: " << uv_strerror(status));
    });
    mServer->Listen(host, port, connectCb);
}

TcpServer::~TcpServer()
{
}
}
}
