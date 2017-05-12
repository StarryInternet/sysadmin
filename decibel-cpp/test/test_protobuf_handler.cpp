#include "decibel-cpp/test/testpb.pb.h"
#include "decibel/Protobuf.h"
#include "decibel/messaging/Protocol.h"
#include <gtest/gtest.h>

using namespace decibel;
using namespace std::placeholders;

#define DEF_SUBHANDLER(TYPE, FIELD)                                            \
public:                                                                        \
    TYPE FIELD##_val;                                                          \
    folly::Future<folly::Unit> Handle_##FIELD(                                 \
        const testpb::TestOneof& message)                                      \
    {                                                                          \
        FIELD##_val = message.FIELD();                                         \
        return folly::makeFuture();                                            \
    }

#define REGISTER_SUBHANDLER(DISPATCHER, FIELD)                                 \
    (DISPATCHER)                                                               \
        .RegisterHandler(                                                      \
            #FIELD,                                                            \
            std::bind(&OneofDispatcherTest::Handle_##FIELD, this, _1));

class OneofDispatcherTest : public ::testing::Test
{
public:
    OneofDispatcherTest() : ::testing::Test()
    {
        REGISTER_SUBHANDLER(mDispatcher, a);
        REGISTER_SUBHANDLER(mDispatcher, b);
        REGISTER_SUBHANDLER(mDispatcher, c);
        REGISTER_SUBHANDLER(mDispatcher, d);
        REGISTER_SUBHANDLER(mDispatcher, e);
        REGISTER_SUBHANDLER(mDispatcher, f);
        REGISTER_SUBHANDLER(mDispatcher, g);
        REGISTER_SUBHANDLER(mDispatcher, h);
    }

    DEF_SUBHANDLER(int32_t, a);
    DEF_SUBHANDLER(int64_t, b);
    DEF_SUBHANDLER(uint32_t, c);
    DEF_SUBHANDLER(uint64_t, d);
    DEF_SUBHANDLER(float, e);
    DEF_SUBHANDLER(double, f);
    DEF_SUBHANDLER(bool, g);
    DEF_SUBHANDLER(std::string, h);

    OneofDispatcher<testpb::TestOneof, void> mDispatcher;
};

#define TEST_ONEOF_SUBHANDLER(TESTNAME, FIELD, ASSERT_MACRO, TEST_VALUE)       \
    TEST_F(OneofDispatcherTest, TESTNAME)                                      \
    {                                                                          \
        testpb::TestOneof message;                                             \
        message.set_##FIELD(TEST_VALUE);                                       \
        auto fut = mDispatcher.Dispatch(std::move(message), "payload");        \
        fut.get();                                                             \
        ASSERT_MACRO(TEST_VALUE, FIELD##_val);                                 \
    }

TEST_ONEOF_SUBHANDLER(OneofInt32, a, ASSERT_EQ, 123);
TEST_ONEOF_SUBHANDLER(OneofInt64, b, ASSERT_EQ, 123);
TEST_ONEOF_SUBHANDLER(OneofUInt32, c, ASSERT_EQ, 123);
TEST_ONEOF_SUBHANDLER(OneofUInt64, d, ASSERT_EQ, 123);
TEST_ONEOF_SUBHANDLER(OneofFloat, e, ASSERT_FLOAT_EQ, 123.123);
TEST_ONEOF_SUBHANDLER(OneofDouble, f, ASSERT_DOUBLE_EQ, 123.123);
TEST_ONEOF_SUBHANDLER(OneofBool, g, ASSERT_EQ, true);
TEST_ONEOF_SUBHANDLER(OneofString, h, ASSERT_EQ, "hello world");
