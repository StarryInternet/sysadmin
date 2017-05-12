
#pragma once

#include <memory>
#include <unordered_set>

#include "decibel/messaging/IProtocolFactory.h"
#include "decibel/niceuv/EventLoop.h"
#include "decibel/niceuv/TcpServer.h"

namespace decibel
{
namespace messaging
{
class TcpServer
{
public:
    typedef std::unordered_set<std::unique_ptr<Protocol>> ProtoContainer;

public:
    TcpServer(niceuv::EventLoop* loop,
              const std::string& host,
              uint16_t port,
              IProtocolFactory* factory);

    ~TcpServer();

private:
    std::unique_ptr<niceuv::TcpServer> mServer;
    ProtoContainer mProtocols;
};
}
}
