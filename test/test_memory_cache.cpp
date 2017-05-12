#include <log4cxx/logger.h>
#include <gtest/gtest.h>

#include "MemoryCache.h"
#include "Helpers.h"

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_memory_storage"));
}

TEST(MemoryStoring, Basic)
{
    MockSystemStorage backing;
    MemoryCache memoryStorage(&backing);

    memoryStorage.Set(ConfigPair("key", ConfigType<std::string>("value")), 0);
    memoryStorage.Commit(0);
    // Sets go straight through to the backing store
    ASSERT_TRUE(UnwrapSinglePair(memoryStorage.Get(ConfigPair::Key("key"))));
    ASSERT_EQ("value", UnwrapSinglePair(backing.Get(ConfigPair::Key("key"))).get().UnpackValue<std::string>());

    // sneakily changing something in the backing store won't be reflected by the in memory store
    // since the memory store doesn't know the key is dirty
    backing.Set(ConfigPair("key", ConfigType<std::string>("newvalue")), 0);
    backing.Commit(0);
    ASSERT_EQ("value", UnwrapSinglePair(memoryStorage.Get(ConfigPair::Key("key"))).get().UnpackValue<std::string>());
}

TEST(MemoryStoring, Erasing)
{
    MockSystemStorage backing;
    MemoryCache memoryStorage(&backing);

    memoryStorage.Set(ConfigPair("key", ConfigType<std::string>("value")), 0);
    memoryStorage.Commit(0);
    ASSERT_TRUE(UnwrapSinglePair(memoryStorage.Get(ConfigPair::Key("key"))));
    ASSERT_EQ("value", UnwrapSinglePair(backing.Get(ConfigPair::Key("key"))).get().UnpackValue<std::string>());

    memoryStorage.Set(ConfigPair("key"), 0);
    memoryStorage.Commit(0);
    ASSERT_FALSE(UnwrapSinglePair(memoryStorage.Get(ConfigPair::Key("key"))));
    ASSERT_FALSE(UnwrapSinglePair(backing.Get(ConfigPair::Key("key"))));

    memoryStorage.Set(ConfigPair("my.ssid", ConfigType<std::string>("test")), 0);
    memoryStorage.Set(ConfigPair("other.value", ConfigType<int32_t>(66)), 0);
    memoryStorage.Set(ConfigPair("other.value"), 0);
    memoryStorage.Set(ConfigPair("not.real"), 0);
    memoryStorage.Commit(0);
    ASSERT_FALSE(UnwrapSinglePair(memoryStorage.Get(ConfigPair::Key("other.value"))));
    ASSERT_FALSE(UnwrapSinglePair(memoryStorage.Get(ConfigPair::Key("not.real"))));
}

TEST(MemoryStoring, GetAllKeys)
{
    MockSystemStorage backing;
    MemoryCache memoryStorage(&backing);

    memoryStorage.Set(ConfigPair("key.house", ConfigType<int32_t>(123)), 0);
    memoryStorage.Set(ConfigPair("key.house.downstairs", ConfigType<std::string>("hey there")), 0);
    memoryStorage.Set(ConfigPair("house.downstairs.tv", ConfigType<bool>(true)), 0);
    memoryStorage.Commit(0);

    auto keys1 = memoryStorage.GetAllKeys(ConfigPair::Key("key.*"));
    REQUIRE_UNORDERED_EQUALS(std::vector<ConfigPair::Key>({ConfigKey("key.house.downstairs"), ConfigKey("key.house")}),
                             keys1);

    auto keys2 = memoryStorage.GetAllKeys(ConfigPair::Key("*"));
    REQUIRE_UNORDERED_EQUALS(std::vector<ConfigPair::Key>({ConfigKey("key.house.downstairs"), ConfigKey("key.house"), ConfigKey("house.downstairs.tv")}),
                             keys2);

    auto keys3 = memoryStorage.GetAllKeys(ConfigPair::Key("key.house.not.real.*"));
    ASSERT_EQ(0, keys3.size());
}
