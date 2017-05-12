
#include <cstring>
#include <iostream>

#include "decibel/niceuv/EventLoop.h"
#include "decibel/niceuv/InterfaceInfo.h"
#include "decibel/niceuv/TcpConn.h"
#include "decibel/niceuv/UvStream.h"

#include <log4cxx/logger.h>

#include "folly/IPAddress.h"

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("TcpConn"));
}

namespace decibel
{
namespace niceuv
{
void on_connect(uv_connect_t* req, int status)
{
    TcpConn* conn = reinterpret_cast<TcpConn*>(req->data);
    conn->ConnectCalled(status);
    delete req;
}

TcpConn::TcpConn(EventLoop* loop)
    : mLoop(loop)
    , mTcpHandler(new UvHandle<uv_tcp_t>)
    , mReadCb()
    , mReadErrorCb()
    , mConnectCb()
    , mConnectErrorCb()
    , mDisconnectCb()
{
    mTcpHandler->Get()->data = this;

    loop->RegisterTcpConn(this);
}

TcpConn::~TcpConn()
{
}

void TcpConn::Reset()
{
    mTcpHandler.reset(new UvHandle<uv_tcp_t>);
    mTcpHandler->Get()->data = this;
    mLoop->RegisterTcpConn(this);
}

void TcpConn::Connect(const std::string& host, uint16_t port)
{
    if (!folly::IPAddress::validate(host))
    {
        mConnectErrorCb.get()(UV_EADDRNOTAVAIL);
        return;
    }
    InterfaceInfo info(host, port);

    uv_connect_t* connectHandler = new uv_connect_t;
    connectHandler->data = this;
    int status = uv_tcp_connect(connectHandler,
                                mTcpHandler->Get(),
                                (struct sockaddr*)&info.GetSockAddr_In(),
                                on_connect);
    if (status != 0 && mConnectErrorCb)
    {
        mConnectErrorCb.get()(status);
    }
}

void TcpConn::RegisterReadCallback(ReadCallback readcb)
{
    mReadCb = boost::make_optional<ReadCallback>(std::move(readcb));
}

void TcpConn::RegisterReadErrorCallback(ReadErrorCallback readErrorCb)
{
    mReadErrorCb = boost::make_optional<ReadErrorCallback>(std::move(readErrorCb));
}

void TcpConn::RegisterConnectCallback(ConnectCallback conncb)
{
    mConnectCb = boost::make_optional<ConnectCallback>(std::move(conncb));
}

void TcpConn::RegisterConnectErrorCallback(ConnectErrorCallback connErrorCb)
{
    mConnectErrorCb = boost::make_optional<ConnectErrorCallback>(std::move(connErrorCb));
}

void TcpConn::RegisterDisconnectCallback(DisconnectCallback disconncb)
{
    mDisconnectCb = boost::make_optional<DisconnectCallback>(std::move(disconncb));
}

void TcpConn::ClearCallbacks()
{
    mReadCb = boost::none;
    mReadErrorCb = boost::none;
    mConnectCb = boost::none;
    mConnectErrorCb = boost::none;
    mDisconnectCb = boost::none;
}

void TcpConn::Start()
{
    uv_read_start((uv_stream_t*)mTcpHandler->Get(), on_alloc, on_read<TcpConn>);
}

void TcpConn::Stop()
{
    uv_read_stop((uv_stream_t*)mTcpHandler->Get());
}

void TcpConn::Write(const TcpConn::Buffer& buf)
{
    Write(buf.data(), buf.size());
}

void TcpConn::Write(const char* data, size_t nwrite)
{
    uv_write_t* write_req(new uv_write_t);
    char* pOwnedBuffer = (char*)malloc(sizeof(char) * nwrite);
    memcpy(pOwnedBuffer, data, nwrite);
    uv_buf_t buf = uv_buf_init(pOwnedBuffer, nwrite);
    write_req->data = pOwnedBuffer;
    uv_write(write_req, (uv_stream_t*)mTcpHandler->Get(), &buf, 1, on_write);
}

void TcpConn::ConnectCalled(int status)
{
    if (status == 0 && mConnectCb)
    {
        mConnectCb.get()(status);
    }
    else if (status != UV_ECANCELED && mConnectErrorCb)
    {
        mConnectErrorCb.get()(status);
    }
}

void TcpConn::ReadCalled(ssize_t nread, const uv_buf_t* buf)
{
    // nread is 0 if socket was closed normally, or negative if an error
    if (nread > 0 && mReadCb)
    {
        TcpConn::Buffer buffer;
        buffer.assign(buf->base, buf->base + nread);
        mReadCb.get()(buffer);
    }
    else if (nread == 0 && mDisconnectCb)
    {
        mDisconnectCb.get()(nread);
    }
    else
    {
        if (mReadErrorCb)
        {
            mReadErrorCb.get()(nread);
        }

        if (mDisconnectCb)
        {
            mDisconnectCb.get()(nread);
        }
    }
}
}
}
