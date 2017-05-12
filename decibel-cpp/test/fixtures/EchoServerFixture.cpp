#include "EchoServerFixture.h"

#include "../Helpers.h"
#include <log4cxx/logger.h>

using namespace decibel::niceuv;
using namespace std::placeholders;

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("EchoServerFixture"));
}

EchoServerFixture::EchoServerFixture(EventLoop* loop)
    : mLoop(loop), mServer(new TcpServer(mLoop)), mClientConn()
{
}

EchoServerFixture::~EchoServerFixture()
{
}

void EchoServerFixture::RunEchoServer(uint16_t port)
{
    mServer->Listen("127.0.0.1",
                    port,
                    std::bind(&EchoServerFixture::ConnCallback, this, _1));
}

void EchoServerFixture::ConnCallback(TcpConnPtr conn)
{
    mClientConn = std::move(conn);
    mClientConn->RegisterReadCallback(
        std::bind(&EchoServerFixture::ReadCallback, this, _1));
    mClientConn->Start();
}

void EchoServerFixture::ReadCallback(const TcpConn::Buffer& buf)
{
    mClientConn->Write(buf);
    mClientConn->Stop();
}
