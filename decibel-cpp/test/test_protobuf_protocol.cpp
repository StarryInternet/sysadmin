#include <gtest/gtest.h>

#include "decibel-cpp/test/testpb.pb.h"
#include "decibel/Protobuf.h"
#include "decibel/messaging/ProtobufOneofProtocol.h"
#include "decibel/messaging/ProtobufProtocol.h"
#include "decibel/messaging/ProtobufProtocolFactory.h"
#include "decibel/messaging/Reactor.h"
#include "decibel/stdlib.h"

using namespace decibel;
using namespace decibel::messaging;
using namespace std::placeholders;

class ProtobufProtocolFixture : public ::testing::Test
{
public:
    ProtobufProtocolFixture() : ::testing::Test(), mDispatcher()
    {
    }

    virtual void SetUp()
    {
        mDispatcher.RegisterHandler(
            "b",
            std::bind(&ProtobufProtocolFixture::HandleBField, this, _1, _2));
    }

    folly::Future<boost::optional<testpb::TestOneof>>
    HandleBField(const testpb::TestOneof& message, Protocol::Id)
    {
        return folly::makeFuture(boost::make_optional(message));
    }

    OneofDispatcher<testpb::TestOneof,
                    boost::optional<testpb::TestOneof>,
                    Protocol::Id>
        mDispatcher;
};

struct TestProtoClientProto
    : public ProtobufOneofProtocol<
        testpb::TestOneof,
        testpb::TestOneof,
        uint16_t
    >
{
    TestProtoClientProto(Reactor* reactor,
                         size_t totalMessageCount,
                         DispatcherT dispatcher)
        : ProtobufOneofProtocol(dispatcher)
        , mMessageCount(totalMessageCount)
        , mpReactor(reactor)
    {
    }

    virtual void OnConnectionLost(int)
    {
        FAIL() << "This should not happen";
    }

    virtual void OnConnectionMade()
    {
        for (size_t i = 0; i < mMessageCount; i++)
        {
            testpb::TestOneof message;
            message.set_b(i);
            Send(message);
        }
    }

    size_t mMessageCount;
    Reactor* mpReactor;
};

struct TestProtoClientProtoFault : public TestProtoClientProto
{
    TestProtoClientProtoFault(Reactor* reactor,
                              size_t totalMessageCount,
                              DispatcherT dispatcher)
        : TestProtoClientProto(reactor, totalMessageCount, dispatcher)
        , mMessagesSent(0)
        , mCalled(false)
    {
    }

    virtual void OnConnectionMade()
    {
        // will fail
        testpb::TestOneof message;
        message.set_h("cb not reached");
        HandleMessage(std::move(message));
    }

    virtual void Send(const google::protobuf::MessageLite& message)
    {
        mMessagesSent++;
        if (!mCalled)
        {
            mCalled = true;
            throw MessageSendError("Failed to send message");
        }
    }

    int mMessagesSent;
    bool mCalled;
};

TEST_F(ProtobufProtocolFixture, ProtocolHandlerDispatch)
{
    Reactor r;
    ProtobufProtocolFactory<
        testpb::TestOneof,
        testpb::TestOneof,
        uint16_t
    > factory(mDispatcher);
    r.ServeTcp("127.0.0.1", 5000, &factory);
    TestProtoClientProto::DispatcherT dispatcher;
    size_t messageCount = 0;
    size_t totalMessageCount = 10;
    dispatcher.RegisterHandler(
        "b", [&](const testpb::TestOneof& message, Protocol::Id) {
            EXPECT_EQ(messageCount, message.b());
            messageCount++;
            if (messageCount == totalMessageCount)
            {
                r.Stop();
            }
            return folly::makeFuture(boost::optional<testpb::TestOneof>());
        });
    auto pProto = std::make_unique<TestProtoClientProto>(
        &r, totalMessageCount, dispatcher);
    r.ConnectTcp("127.0.0.1", 5000, std::move(pProto));
    r.Start();
}

TEST_F(ProtobufProtocolFixture, SendErrorCallback)
{
    Reactor r;
    ProtobufProtocolFactory<
        testpb::TestOneof,
        testpb::TestOneof,
        uint16_t
    > factory(mDispatcher);
    r.ServeTcp("127.0.0.1", 5000, &factory);
    TestProtoClientProtoFault::DispatcherT dispatcher;
    size_t messageCount = 0;
    size_t totalMessageCount = 2;
    dispatcher.RegisterHandler(
        "h", [&](const testpb::TestOneof& message, Protocol::Id) {
            r.Stop();
            return folly::makeFuture(
                boost::optional<testpb::TestOneof>(message)
            );
        });
    auto pProto = std::make_unique<TestProtoClientProtoFault>(
        &r, totalMessageCount, dispatcher);

    pProto->RegisterSendErrorCallback(
        [](
            ProtobufOneofProtocol<
                testpb::TestOneof,
                testpb::TestOneof,
                uint16_t
            >* proto,
            const testpb::TestOneof&)
        {
            testpb::TestOneof reply;
            reply.set_h("cb reached");
            proto->Send(reply);
        });
    auto protoCopy = pProto.get();
    r.ConnectTcp("127.0.0.1", 5000, std::move(pProto));
    r.Start();
    ASSERT_EQ(2, protoCopy->mMessagesSent);
}

TEST(FramedBuffer, ZeroSize)
{
    using Buffer = FramedBuffer<uint16_t>;
    Buffer b;
    char data[sizeof(Buffer::LengthType)];
    Buffer::LengthType size = 0;
    memcpy(data, &size, sizeof(Buffer::LengthType));
    EXPECT_FALSE(b.Ready());
    EXPECT_EQ(sizeof(Buffer::LengthType),
              b.AddData(data, sizeof(Buffer::LengthType)));
    EXPECT_EQ(0, b.Size());
    EXPECT_TRUE(b.Ready());
}

TEST(FramedBuffer, ZeroSizeExtraData)
{
    using Buffer = FramedBuffer<uint16_t>;
    Buffer b;
    const char* as = "aaaa";
    char data[sizeof(Buffer::LengthType) + sizeof(char) * 4];
    Buffer::LengthType size = 0;
    memcpy(data, &size, sizeof(Buffer::LengthType));
    memcpy(data + sizeof(Buffer::LengthType), as, sizeof(char) * 4);
    EXPECT_FALSE(b.Ready());
    EXPECT_EQ(sizeof(Buffer::LengthType),
              b.AddData(data, sizeof(Buffer::LengthType)));
    EXPECT_EQ(0, b.Size());
    EXPECT_TRUE(b.Ready());
}

TEST(FramedBuffer, SizeMultipleMessages)
{
    using Buffer = FramedBuffer<uint16_t>;
    Buffer::LengthType size = 10;;
    char data[sizeof(Buffer::LengthType)];
    memcpy(data, &size, sizeof(Buffer::LengthType));
    Buffer b;
    for (size_t i = 0; i < sizeof(Buffer::LengthType); i++)
    {
        EXPECT_EQ(0, b.Size());
        EXPECT_FALSE(b.Ready());
        EXPECT_EQ(1, b.AddData(data + i, 1));
    }
    EXPECT_EQ(10, b.Size());
}

TEST(FramedBuffer, AddRealData)
{
    using Buffer = FramedBuffer<uint16_t>;
    Buffer b;
    const char* as = "aaaa";
    size_t total_size = sizeof(Buffer::LengthType) + sizeof(char) * 4;
    char data[total_size];
    Buffer::LengthType size = 4;
    memcpy(data, &size, sizeof(Buffer::LengthType));
    memcpy(data + sizeof(Buffer::LengthType), as, sizeof(char) * 4);
    EXPECT_FALSE(b.Ready());
    EXPECT_EQ(total_size, b.AddData(data, total_size));
    EXPECT_EQ(size, b.Size());
    EXPECT_TRUE(b.Ready());
    EXPECT_EQ(0, strncmp(as, b.Data(), size));
}
