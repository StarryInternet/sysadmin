#include <log4cxx/logger.h>
#include <gtest/gtest.h>

#include "ProtobufTranslator.h"
#include "ConfigTypes.h"

#include "sysadmin-api/sysadminctl.pb.h"

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_protobuf_translator"));
}

#define TEST_SIMPLE_SET(field, value, unpacktype)                \
    TEST(FromProtoTranslation, BasicSet##field)                  \
    {                                                            \
        ProtobufTranslator translator;                           \
        sysadminctl::Command command;                            \
        auto confvalue = command.mutable_set()->mutable_value(); \
        confvalue->set_##field(value);                           \
        command.mutable_set()->set_key("waffle.house");          \
        auto pair = translator.TranslateToSet(command).get();    \
        ASSERT_EQ("waffle.house", pair.GetKey().ToString());     \
        ASSERT_EQ(value, pair.UnpackValue<unpacktype>());        \
    }

#define TEST_GET_RESPONSE(field, expvalue, packtype)                             \
    TEST(ToProtoTranslation, GetResponse##field)                                 \
    {                                                                            \
        ProtobufTranslator translator;                                           \
        auto pair = ConfigPair("waffle.house", ConfigType<packtype>(expvalue));  \
        auto message = translator.TranslateFromGet({pair});                      \
        ASSERT_EQ(1, message.get().kvs_size());                                  \
        ASSERT_EQ(expvalue, message.get().kvs(0).value().field());               \
    }

TEST(FromProtoTranslation, BasicGet)
{
    ProtobufTranslator t;
    sysadminctl::Command message;
    message.mutable_get()->set_key("waffle.house");
    auto key = t.TranslateToGet(message).get();
    ASSERT_EQ("waffle.house", key.ToString());

    sysadminctl::Command message2;
    // set, not get
    message2.mutable_set()->set_key("waffle.house");
    auto pair2 = t.TranslateToGet(message2);
    ASSERT_FALSE(pair2);
}

TEST(ToProtoTranslation, EmptyGetResponse)
{
    ProtobufTranslator t;
    auto emptied = ConfigPair("My Value is empty");
    auto resp = t.TranslateFromGet({emptied});
    ASSERT_EQ(0, resp.get().kvs_size());
}

TEST_SIMPLE_SET(int32val, -4, int32_t);
TEST_SIMPLE_SET(strval, "cheese", std::string);
TEST_SIMPLE_SET(boolval, true, bool);

TEST_GET_RESPONSE(int32val, -4, int32_t);
TEST_GET_RESPONSE(strval, "cheese", std::string);
TEST_GET_RESPONSE(boolval, true, bool);

TEST(FromProtoTranslation, IntList)
{
    ProtobufTranslator translator;
    sysadminctl::Command command;
    command.mutable_set()->set_key("list");
    auto confvalue = command.mutable_set()->mutable_value();
    auto list = confvalue->mutable_int32list();
    list->add_list(4);
    list->add_list(5);
    list->add_list(6);
    auto pair = translator.TranslateToSet(command).get();
    ASSERT_EQ(std::vector<int32_t>({4, 5, 6}), pair.UnpackValue<std::vector<int32_t>>());
}

TEST(FromProtoTranslation, BoolList)
{
    ProtobufTranslator translator;
    sysadminctl::Command command;
    command.mutable_set()->set_key("list");
    auto confvalue = command.mutable_set()->mutable_value();
    auto list = confvalue->mutable_boollist();
    list->add_list(true);
    list->add_list(false);
    list->add_list(false);
    auto pair = translator.TranslateToSet(command).get();
    ASSERT_EQ(std::vector<bool>({true, false, false}), pair.UnpackValue<std::vector<bool>>());
}

TEST(FromProtoTranslation, StringList)
{
    ProtobufTranslator translator;
    sysadminctl::Command command;
    command.mutable_set()->set_key("list");
    auto confvalue = command.mutable_set()->mutable_value();
    auto list = confvalue->mutable_stringlist();
    list->add_list("hey");
    list->add_list("there");
    list->add_list("world");
    auto pair = translator.TranslateToSet(command).get();
    ASSERT_EQ(std::vector<std::string>({"hey", "there", "world"}), pair.UnpackValue<std::vector<std::string>>());
}

TEST(ToProtoTranslation, IntList)
{
    ProtobufTranslator translator;
    std::vector<int32_t> list = {4, 5, 6};
    auto pair = ConfigPair("key", ConfigType<std::vector<int32_t>>(list));
    auto message = translator.TranslateFromGet({pair});
    ASSERT_EQ(4, message.get().kvs(0).value().int32list().list(0));
    ASSERT_EQ(5, message.get().kvs(0).value().int32list().list(1));
    ASSERT_EQ(6, message.get().kvs(0).value().int32list().list(2));
}

TEST(ToProtoTranslation, BoolList)
{
    ProtobufTranslator translator;
    std::vector<bool> list = {true, false, false};
    auto pair = ConfigPair("key", ConfigType<std::vector<bool>>(list));
    auto message = translator.TranslateFromGet({pair});
    ASSERT_EQ(true, message.get().kvs(0).value().boollist().list(0));
    ASSERT_EQ(false, message.get().kvs(0).value().boollist().list(1));
    ASSERT_EQ(false, message.get().kvs(0).value().boollist().list(2));
}

TEST(ToProtoTranslation, StringList)
{
    ProtobufTranslator translator;
    std::vector<std::string> list = {"hey", "there", "world"};
    auto pair = ConfigPair("key", ConfigType<std::vector<std::string>>(list));
    auto message = translator.TranslateFromGet({pair});
    ASSERT_EQ("hey", message.get().kvs(0).value().stringlist().list(0));
    ASSERT_EQ("there", message.get().kvs(0).value().stringlist().list(1));
    ASSERT_EQ("world", message.get().kvs(0).value().stringlist().list(2));
}

TEST(ToProtoTranslation, GetAllKeys)
{
    ProtobufTranslator translator;
    std::vector<ConfigPair> list({ConfigPair("key.house", ConfigType<int32_t>(0)),
                                  ConfigPair("house.tv", ConfigType<bool>(true))});
    auto message = translator.TranslateFromGet(list);
    ASSERT_EQ("key.house", message.get().kvs(0).key());
    ASSERT_EQ("house.tv", message.get().kvs(1).key());
    ASSERT_EQ(2, message.get().kvs_size());
}
