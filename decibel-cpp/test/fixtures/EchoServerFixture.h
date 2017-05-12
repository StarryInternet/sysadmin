#pragma once

#include <memory>

#include "decibel/niceuv/EventLoop.h"
#include "decibel/niceuv/TcpConn.h"
#include "decibel/niceuv/TcpServer.h"

#include <signal.h>

#include <gtest/gtest.h>

struct EchoServerFixture
{
    EchoServerFixture(decibel::niceuv::EventLoop* loop);

    virtual ~EchoServerFixture();

    void RunEchoServer(uint16_t port);

    void ConnCallback(decibel::niceuv::TcpConnPtr conn);

    void ReadCallback(const decibel::niceuv::TcpConn::Buffer& buf);

    decibel::niceuv::EventLoop* mLoop;
    std::unique_ptr<decibel::niceuv::TcpServer> mServer;
    decibel::niceuv::TcpConnPtr mClientConn;
};