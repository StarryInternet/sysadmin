#include <log4cxx/logger.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstdio>

#include "LocalYAMLStorage.h"
#include "Helpers.h"

namespace bfs = boost::filesystem;

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_local_yaml_storage"));
}

TEST(KeyTranslatorTest, Basic)
{
    auto path = TranslateKeyToPathPair(ConfigPair::Key("waffle.house"));
    ASSERT_EQ("waffle.yaml", path.first.string());
    ASSERT_EQ("house", path.second);

    auto shortpath = TranslateKeyToPathPair(ConfigPair::Key("waffle"));
    // Key's with no leading file name are placed in the "global" config file
    ASSERT_EQ("global.yaml", shortpath.first.string());
    ASSERT_EQ("waffle", shortpath.second);

    auto longerpath = TranslateKeyToPathPair(ConfigPair::Key("waffle.cheese.sandwich"));
    ASSERT_EQ("waffle/cheese.yaml", longerpath.first.string());
    ASSERT_EQ("sandwich", longerpath.second);
}

TEST(KeyTranslatorTest, BasicReverse)
{
    auto key = TranslatePathToKey("", "path/to/value.yaml", "key");
    ASSERT_EQ("path.to.value.key", key);

    auto rootkey = TranslatePathToKey("/some/root/path", "/some/root/path/to/a/value.yaml", "key");
    ASSERT_EQ("to.a.value.key", rootkey);

    auto shortkey = TranslatePathToKey("/root/path", "/root/path/global.yaml", "key");
    ASSERT_EQ("key", shortkey);
}

TEST(TestYAMLTranslation, Basic)
{
    auto node = YAML::Load("astring");
    auto value = ValueFromYAML("string", node);
    ASSERT_EQ("astring", value.Unpack<std::string>());

    auto node2 = YAML::Load("true");
    auto value2 = ValueFromYAML("bool", node2);
    ASSERT_EQ(true, value2.Unpack<bool>());

    auto node3 = YAML::Load("\"true\"");
    auto value3 = ValueFromYAML("bool", node3);
    ASSERT_EQ(true, value3.Unpack<bool>());

    // we can force the type if we know it ahead of time
    auto node4 = YAML::Load("123");
    auto value4 = ValueFromYAML("string", node4);
    ASSERT_EQ("123", value4.Unpack<std::string>());
}

TEST_F(LocalWritingFixture, Basic)
{
    LocalYAMLStorage storage(mTmpPath);
    storage.Set(ConfigPair("key.house", ConfigType<int32_t>(123)), 0);
    storage.Commit(0);
    ASSERT_TRUE(bfs::exists(mTmpPath / "key.yaml"));
    ASSERT_EQ(123, UnwrapSinglePair(storage.Get(ConfigPair::Key("key.house"))).get().UnpackValue<int32_t>());

    storage.Set(ConfigPair("global", ConfigType<std::string>("hey there")), 0);
    storage.Commit(0);
    ASSERT_TRUE(bfs::exists(mTmpPath / "global.yaml"));
    ASSERT_EQ("hey there", UnwrapSinglePair(storage.Get(ConfigPair::Key("global"))).get().UnpackValue<std::string>());

    storage.Set(ConfigPair("really.long.path.sorta", ConfigType<bool>(true)), 0);
    storage.Commit(0);
    ASSERT_TRUE(bfs::exists(mTmpPath / "really" / "long" / "path.yaml"));
    ASSERT_EQ(true, UnwrapSinglePair(storage.Get(ConfigPair::Key("really.long.path.sorta"))).get().UnpackValue<bool>());

    storage.Set(ConfigPair("really.long.variable", ConfigType<bool>(true)), 0);
    storage.Commit(0);
    ASSERT_TRUE(bfs::exists(mTmpPath / "really" / "long.yaml"));
    ASSERT_EQ(true, UnwrapSinglePair(storage.Get(ConfigPair::Key("really.long.variable"))).get().UnpackValue<bool>());
    ASSERT_TRUE(bfs::exists(mTmpPath / "really" / "long" / "path.yaml"));

    ASSERT_FALSE(UnwrapSinglePair(storage.Get(ConfigPair::Key("this.doesnt.exist.com"))));
}

TEST_F(LocalWritingFixture, BasicChecking)
{
    LocalYAMLStorage storage(mTmpPath);
    storage.Set(ConfigPair("key.house", ConfigType<int32_t>(123)), 0);
    storage.Commit(0);
    ASSERT_TRUE(storage.TypeCheck(ConfigPair("key.house", ConfigType<int32_t>(0))));
    ASSERT_TRUE(storage.TypeCheck(ConfigPair("key.doesntexist", ConfigType<bool>(true))));
    ASSERT_FALSE(storage.TypeCheck(ConfigPair("key.house", ConfigType<std::string>("0"))));

    ASSERT_TRUE(UnwrapSinglePair(storage.Get(ConfigPair::Key("key.house"))));
    ASSERT_FALSE(UnwrapSinglePair(storage.Get(ConfigPair::Key("key.doesntexist"))));
}

TEST_F(LocalWritingFixture, GroupSetTests)
{
    LocalYAMLStorage storage(mTmpPath);
    ConfigPairList configlist = {ConfigPair("my.ssid", ConfigType<std::string>("test")),
                                 ConfigPair("other.value", ConfigType<int32_t>(66)),
                                 ConfigPair("something.else", ConfigType<bool>(true))};

    storage.GroupSet(configlist);
    ASSERT_EQ("test", UnwrapSinglePair(storage.Get(ConfigPair::Key("my.ssid"))).get().UnpackValue<std::string>());
    ASSERT_EQ(66, UnwrapSinglePair(storage.Get(ConfigPair::Key("other.value"))).get().UnpackValue<int32_t>());
    ASSERT_EQ(true, UnwrapSinglePair(storage.Get(ConfigPair::Key("something.else"))).get().UnpackValue<bool>());

    ConfigPairList configlist2 = {ConfigPair("my.ssid.is.this", ConfigType<std::string>("waffle")),
                                  ConfigPair("my.ssid.is.not", ConfigType<std::string>("cheese")),
                                  ConfigPair("my.value.is", ConfigType<int32_t>(33)),
                                  ConfigPair("my.other.else", ConfigType<bool>(false))};

    storage.GroupSet(configlist2);
    ASSERT_EQ("waffle", UnwrapSinglePair(storage.Get(ConfigPair::Key("my.ssid.is.this"))).get().UnpackValue<std::string>());
    ASSERT_EQ("cheese", UnwrapSinglePair(storage.Get(ConfigPair::Key("my.ssid.is.not"))).get().UnpackValue<std::string>());
    ASSERT_EQ(33, UnwrapSinglePair(storage.Get(ConfigPair::Key("my.value.is"))).get().UnpackValue<int32_t>());
    ASSERT_EQ(false, UnwrapSinglePair(storage.Get(ConfigPair::Key("my.other.else"))).get().UnpackValue<bool>());

    // This list erases the value at 'other.value'
    ConfigPairList configlist3 = {ConfigPair("my.ssid", ConfigType<std::string>("wifi")),
                                  ConfigPair("other.value")};
    storage.GroupSet(configlist3);
    ASSERT_EQ("wifi", UnwrapSinglePair(storage.Get(ConfigPair::Key("my.ssid"))).get().UnpackValue<std::string>());
    ASSERT_FALSE(UnwrapSinglePair(storage.Get(ConfigPair::Key("other.value"))));
}

TEST_F(LocalWritingFixture, ListTests)
{
    LocalYAMLStorage storage(mTmpPath);
    auto intlist = ConfigPair("my.int.list", ConfigType<std::vector<int32_t>>({4, 5, 6}));
    auto boollist = ConfigPair("my.bool.list", ConfigType<std::vector<bool>>({true, false}));
    auto stringlist = ConfigPair("my.string.list", ConfigType<std::vector<std::string>>({"a string"}));

    storage.Set(intlist, 0);
    storage.Set(boollist, 0);
    storage.Set(stringlist, 0);
    storage.Commit(0);
    ASSERT_EQ(std::vector<int32_t>({4, 5, 6}),
              UnwrapSinglePair(storage.Get(ConfigPair::Key("my.int.list"))).get().UnpackValue<std::vector<int32_t>>());
    ASSERT_EQ(std::vector<bool>({true, false}),
              UnwrapSinglePair(storage.Get(ConfigPair::Key("my.bool.list"))).get().UnpackValue<std::vector<bool>>());
    ASSERT_EQ(std::vector<std::string>({"a string"}),
              UnwrapSinglePair(storage.Get(ConfigPair::Key("my.string.list"))).get().UnpackValue<std::vector<std::string>>());

    auto intlist2 = ConfigPair("my.int.list", ConfigType<std::vector<int32_t>>({1, 2, 3, 4}));
    storage.Set(intlist2, 0);
    storage.Commit(0);
    ASSERT_EQ(std::vector<int32_t>({1, 2, 3, 4}),
              UnwrapSinglePair(storage.Get(ConfigPair::Key("my.int.list"))).get().UnpackValue<std::vector<int32_t>>());
}

TEST_F(LocalWritingFixture, GetAllTests)
{
    LocalYAMLStorage storage(mTmpPath);
    ASSERT_EQ(0, storage.GetAll().size());

    storage.Set(ConfigPair("key.house", ConfigType<int32_t>(123)), 0);
    storage.Set(ConfigPair("global", ConfigType<std::string>("hey there")), 0);
    storage.Set(ConfigPair("really.long.path.sorta", ConfigType<bool>(true)), 0);
    storage.Set(ConfigPair("really.long.variable", ConfigType<bool>(true)), 0);
    storage.Set(ConfigPair("really.long.variable", ConfigType<bool>(false)), 0);
    storage.Commit(0);

    auto all = storage.GetAll();
    ASSERT_EQ(1, all.count(ConfigPair::Key("key.house")));
    ASSERT_EQ(1, all.count(ConfigPair::Key("global")));
    ASSERT_EQ(1, all.count(ConfigPair::Key("really.long.path.sorta")));
    ASSERT_EQ(1, all.count(ConfigPair::Key("really.long.variable")));
    ASSERT_EQ(false, all.at(ConfigPair::Key("really.long.variable")).UnpackValue<bool>());
}

TEST_F(LocalWritingFixture, EraseTests)
{
    LocalYAMLStorage storage(mTmpPath);

    storage.Set(ConfigPair("key.house", ConfigType<int32_t>(123)), 0);
    storage.Set(ConfigPair("global", ConfigType<std::string>("hey there")), 0);
    storage.Set(ConfigPair("really.long.path.sorta", ConfigType<bool>(true)), 0);
    storage.Commit(0);

    ASSERT_EQ(123, UnwrapSinglePair(storage.Get(ConfigPair::Key("key.house"))).get().UnpackValue<int32_t>());
    storage.Set(ConfigPair("key.house"), 0);
    storage.Commit(0);
    ASSERT_FALSE(UnwrapSinglePair(storage.Get(ConfigPair::Key("key.house"))));

    // Nothing should happen if we set an empty config pair for a key that doesn't exist
    storage.Set(ConfigPair("not.real.key"), 0);
    ASSERT_FALSE(UnwrapSinglePair(storage.Get(ConfigPair::Key("not.real.key"))));
}

TEST_F(LocalWritingFixture, GetAllKeys)
{
    LocalYAMLStorage storage(mTmpPath);

    storage.Set(ConfigPair("key.house", ConfigType<int32_t>(123)), 0);
    storage.Set(ConfigPair("key.house.downstairs", ConfigType<std::string>("hey there")), 0);
    storage.Set(ConfigPair("house.downstairs.tv", ConfigType<bool>(true)), 0);
    storage.Commit(0);

    auto keys1 = storage.GetAllKeys(ConfigPair::Key("key.*"));
    REQUIRE_UNORDERED_EQUALS(std::vector<ConfigPair::Key>({ConfigKey("key.house.downstairs"), ConfigKey("key.house")}),
                             keys1);

    auto keys2 = storage.GetAllKeys(ConfigPair::Key("*"));
    REQUIRE_UNORDERED_EQUALS(std::vector<ConfigPair::Key>({ConfigKey("key.house.downstairs"), ConfigKey("key.house"), ConfigKey("house.downstairs.tv")}),
                             keys2);

    auto keys3 = storage.GetAllKeys(ConfigPair::Key("key.house.not.real.*"));
    ASSERT_EQ(0, keys3.size());
}

TEST_F(LocalWritingFixture, GetWildcards)
{
    LocalYAMLStorage storage(mTmpPath);

    storage.Set(ConfigPair("key.house", ConfigType<int32_t>(123)), 0);
    storage.Set(ConfigPair("key.house.downstairs", ConfigType<std::string>("hey there")), 0);
    storage.Set(ConfigPair("house.downstairs.tv", ConfigType<bool>(true)), 0);
    storage.Commit(0);

    auto keys = storage.Get(ConfigPair::Key("key.*"));
    ASSERT_EQ("hey there", keys.at(ConfigPair::Key("key.house.downstairs")).UnpackValue<std::string>());
    ASSERT_EQ(123, keys.at(ConfigPair::Key("key.house")).UnpackValue<int32_t>());
    ASSERT_EQ(2, keys.size());

    auto keys2 = storage.Get(ConfigPair::Key("house.*"));
    ASSERT_EQ(true, keys2.at(ConfigPair::Key("house.downstairs.tv")).UnpackValue<bool>());
    ASSERT_EQ(1, keys2.size());
}
