
#pragma once

#include <uv.h>

#include "TcpConn.h"
#include "UvHandle.h"

namespace decibel
{
namespace niceuv
{
// This is private inheritance for practical reasons. The TcpServer is
// effectively a TcpConn,
// but when communicating with clients you do not use the TcpServer, you use the
// TcpConns that
// the TcpServer provides, so we do not want to provide access to TcpConn's
// read/write methods
// via the TcpServer
class TcpServer : private TcpConn
{
    friend void on_connection(uv_stream_t* server, int status);

public:
    typedef std::function<void(TcpConnPtr conn)> ConnectionCallback;
    typedef std::function<void(int status)> FatalErrorCallback;

public:
    TcpServer(EventLoop* loop);

    ~TcpServer();

    void RegisterFatalCallback(FatalErrorCallback fatalcb);

    void
    Listen(const std::string& ip, uint16_t port, ConnectionCallback conncb);

private:
    void ConnectionReceived(int status);

private:
    EventLoop* mEventLoop;
    ConnectionCallback mConnCb;
    FatalErrorCallback mFatalCb;
    size_t mClientQueueLen;
};
}
}
