
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include <uv.h>

#include "UvHandle.h"

#include <boost/optional.hpp>

namespace decibel
{
namespace niceuv
{
class EventLoop;

class TcpConn
{
    // I'm lazy, and mostly I never want to directly expose the libuv types
    friend class EventLoop;
    friend class TcpServer;
    friend void on_connect(uv_connect_t* req, int status);

    template <typename T>
    friend void
    on_read(uv_stream_t* handler, ssize_t nread, const uv_buf_t* buf);
    friend void on_write(uv_write_t* req, int status);
    friend void on_alloc(uv_handle_t* req, size_t sug_size, uv_buf_t* buf);

public:
    typedef std::vector<char> Buffer;

    typedef std::function<void(const Buffer&)> ReadCallback;
    typedef std::function<void(int)> ReadErrorCallback;
    typedef std::function<void(int)> ConnectCallback;
    typedef std::function<void(int)> ConnectErrorCallback;
    typedef std::function<void(int)> ConnectionCallback;
    typedef std::function<void(int)> DisconnectCallback;

public:
    TcpConn(EventLoop* loop);

    ~TcpConn();

    void Reset();

    void Connect(const std::string& ip, uint16_t port);

    void RegisterReadCallback(ReadCallback readcb);

    void RegisterReadErrorCallback(ReadErrorCallback readErrorCb);

    void RegisterConnectCallback(ConnectCallback conncb);

    void RegisterConnectErrorCallback(ConnectErrorCallback connErrorCb);

    void RegisterDisconnectCallback(DisconnectCallback disconncb);

    void ClearCallbacks();

    void Start();

    void Stop();

    void Write(const Buffer& buf);

    // Silly Kyle
    void Write(const char* buf, size_t nwrite);

    // Internal callback handlers
private:
    void ConnectCalled(int status);

    void ReadCalled(ssize_t nread, const uv_buf_t* buf);

private:
    EventLoop* mLoop;
    std::unique_ptr<UvHandle<uv_tcp_t>> mTcpHandler;
    boost::optional<ReadCallback> mReadCb;
    boost::optional<ReadErrorCallback> mReadErrorCb;
    boost::optional<ConnectCallback> mConnectCb;
    boost::optional<ConnectErrorCallback> mConnectErrorCb;
    boost::optional<DisconnectCallback> mDisconnectCb;
};

typedef std::unique_ptr<TcpConn> TcpConnPtr;
}
}
