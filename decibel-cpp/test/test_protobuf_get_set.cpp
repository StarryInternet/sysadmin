
#include "decibel-cpp/test/testpb.pb.h"
#include "decibel/Protobuf.h"
#include <gtest/gtest.h>

using namespace decibel;

#define TEST_GET_SET_FIELD(NAME, FIELD, ASSERT_MACRO, VALUE)                   \
    TEST(ProtobufFieldGetSet, NAME)                                            \
    {                                                                          \
        testpb::TestOneof message;                                             \
        SetMessageField(&message, #FIELD, VALUE);                              \
        auto actual = GetMessageField<decltype(VALUE)>(message, #FIELD);       \
        ASSERT_MACRO(VALUE, actual);                                           \
    }

TEST_GET_SET_FIELD(Int32, a, ASSERT_EQ, 123);
TEST_GET_SET_FIELD(Int64, b, ASSERT_EQ, 123l);
TEST_GET_SET_FIELD(UInt32, c, ASSERT_EQ, 123u);
TEST_GET_SET_FIELD(UInt64, d, ASSERT_EQ, 123ul);
TEST_GET_SET_FIELD(Float, e, ASSERT_FLOAT_EQ, 123.123f);
TEST_GET_SET_FIELD(Double, f, ASSERT_DOUBLE_EQ, 123.123);
TEST_GET_SET_FIELD(Bool, g, ASSERT_EQ, true);
TEST_GET_SET_FIELD(String, h, ASSERT_EQ, std::string("hello world"));
