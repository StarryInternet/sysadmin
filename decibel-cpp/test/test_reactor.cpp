#include <gtest/gtest.h>
#include <log4cxx/logger.h>

#include "Helpers.h"
#include "fixtures/EchoServerFixture.h"

#include "decibel/messaging/IProtocolFactory.h"
#include "decibel/messaging/Protocol.h"
#include "decibel/messaging/ProtocolFactory.h"
#include "decibel/messaging/RabbitMQMessageHandler.h"
#include "decibel/messaging/RabbitMQProtocol.h"
#include "decibel/messaging/Reactor.h"
#include "decibel/messaging/TcpServer.h"
#include "decibel/stdlib.h"

namespace dm = decibel::messaging;
using namespace std::placeholders;

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_reactor"));
}

class TestTcpProtocol : public dm::Protocol
{
public:
    TestTcpProtocol(dm::Reactor* pReactor)
        : Protocol()
        , mpReactor(pReactor)
        , mConnectionLostCalls(0)
        , mDataReceivedCalls(0)
        , mConnectionMadeCalls(0)
        , mTestStr("hello world")
    {
    }

    virtual void OnConnectionLost(int)
    {
        mConnectionLostCalls += 1;
        mpReactor->Stop();
    }

    virtual void OnDataReceived(const std::vector<char>&)
    {
        mDataReceivedCalls += 1;
        mpTransport->Disconnect();
    }

    virtual void OnConnectionMade()
    {
        mConnectionMadeCalls += 1;
        mpTransport->Write(mTestStr.c_str(), mTestStr.size());
    }

    dm::Reactor* mpReactor;
    int mConnectionLostCalls;
    int mDataReceivedCalls;
    int mConnectionMadeCalls;
    const std::string mTestStr;
};

struct TestTcpEchoProtocol : public dm::Protocol
{
    TestTcpEchoProtocol()
    {
    }

    virtual void OnConnectionLost(int)
    {
        FAIL() << "This should not happen";
    }

    virtual void OnDataReceived(const std::vector<char>& buf)
    {
        mpTransport->Write(buf.data(), buf.size());
    }

    virtual void OnConnectionMade()
    {
    }
};

void RegisterTimeout(dm::Reactor* r, size_t timeout)
{
    r->CallLater(timeout, [r, timeout]() {
        r->Stop();
        FAIL() << "Test timed out after " << timeout << "ms";
    });
}

TEST(TestReactor, CallLater)
{
    dm::Reactor r;
    bool called_one = false;
    int called_one_with = 0;
    r.CallLater(10, [&]() { FAIL() << "Should not be called"; });
    r.CallLater(1,
                std::bind(
                    [&](int x) -> int {
                        called_one = true;
                        return x;
                    },
                    5))
        .then([&](int x) {
            ASSERT_TRUE(called_one);
            called_one_with = x;
        });
    r.CallLater(3, [&]() {
        ASSERT_TRUE(called_one);
        ASSERT_EQ(called_one_with, 5);
        r.Stop();
    });
    r.Start();

    ASSERT_TRUE(called_one);
}

TEST(TestSelfConnect, ConnectTcp)
{
    dm::Reactor r;
    auto pProto = std::make_unique<TestTcpProtocol>(&r);
    auto rawpProto = pProto.get();
    dm::ProtocolFactory<TestTcpEchoProtocol> factory;
    r.ServeTcp("127.0.0.1", 5000, &factory);
    r.ConnectTcp("127.0.0.1", 5000, std::move(pProto));

    r.Start();

    ASSERT_EQ(1, rawpProto->mConnectionMadeCalls);
    ASSERT_EQ(1, rawpProto->mDataReceivedCalls);
    ASSERT_EQ(1, rawpProto->mConnectionLostCalls);
}

TEST(TestSelfConnectMultiClients, ConnectTcp)
{
    dm::Reactor r;
    auto pProto = std::make_unique<TestTcpProtocol>(&r);
    auto rawpProto = pProto.get();
    auto pProto2 = std::make_unique<TestTcpProtocol>(&r);
    auto rawpProto2 = pProto2.get();
    dm::ProtocolFactory<TestTcpEchoProtocol> factory;
    r.ServeTcp("127.0.0.1", 5000, &factory);
    r.ConnectTcp("127.0.0.1", 5000, std::move(pProto));
    r.ConnectTcp("127.0.0.1", 5000, std::move(pProto2));

    r.Start();

    ASSERT_EQ(1, rawpProto->mConnectionMadeCalls);
    ASSERT_EQ(1, rawpProto->mDataReceivedCalls);
    ASSERT_EQ(1, rawpProto->mConnectionLostCalls);
    ASSERT_EQ(1, rawpProto2->mConnectionMadeCalls);
    ASSERT_EQ(1, rawpProto2->mDataReceivedCalls);
    ASSERT_EQ(1, rawpProto2->mConnectionLostCalls);
}

TEST(TestReactor, FollyExecutorAdd)
{
    dm::Reactor r;
    bool then_called = false;
    auto fut = folly::makeFuture().via(&r).then([&]() { then_called = true; });
    ASSERT_FALSE(then_called);
    r.Start();
    ASSERT_TRUE(then_called);
}
