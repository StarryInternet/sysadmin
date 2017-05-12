
#include <cassert>

#include "decibel/niceuv/EventLoop.h"
#include "decibel/niceuv/InterfaceInfo.h"
#include "decibel/niceuv/TcpServer.h"

#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("TcpServer"));
}

namespace decibel
{
namespace niceuv
{
#define CALL_IF_FATAL(callback, status)                                        \
    if (status != 0)                                                           \
    {                                                                          \
        if (callback)                                                          \
        {                                                                      \
            callback(status);                                                  \
        }                                                                      \
    }

void on_connection(uv_stream_t* server_stream, int status)
{
    TcpServer* server = reinterpret_cast<TcpServer*>(server_stream->data);
    server->ConnectionReceived(status);
}

TcpServer::TcpServer(EventLoop* loop)
    : TcpConn(loop)
    , mEventLoop(loop)
    , mConnCb()
    , mFatalCb()
    , mClientQueueLen(10)
{
    // This is overriding the data pointer of the superclass tcp handle
    mTcpHandler->Get()->data = this;
}

TcpServer::~TcpServer()
{
}

void TcpServer::RegisterFatalCallback(FatalErrorCallback fatalcb)
{
    mFatalCb = fatalcb;
}

void TcpServer::Listen(const std::string& ip,
                       uint16_t port,
                       ConnectionCallback conncb)
{
    mConnCb = conncb;

    InterfaceInfo info(ip, port);
    int ret = uv_tcp_bind(
        mTcpHandler->Get(), (struct sockaddr*)&info.GetSockAddr_In(), 0);
    CALL_IF_FATAL(mFatalCb, ret);

    ret = uv_listen(
        (uv_stream_t*)mTcpHandler->Get(), mClientQueueLen, on_connection);
    CALL_IF_FATAL(mFatalCb, ret);
}

void TcpServer::ConnectionReceived(int status)
{
    CALL_IF_FATAL(mFatalCb, status);

    TcpConnPtr client(new TcpConn(mEventLoop));
    int ret = uv_accept((uv_stream_t*)mTcpHandler->Get(),
                        (uv_stream_t*)client->mTcpHandler->Get());
    CALL_IF_FATAL(mFatalCb, ret);
    mConnCb(std::move(client));
}
}
}
