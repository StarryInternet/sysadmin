#include <log4cxx/logger.h>
#include <gtest/gtest.h>

#include "ConfigTypes.h"

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_config_types"));
}

TEST(TestConfigTypes, Construction)
{
    auto val = ConfigValue(ConfigType<std::string>("not satisfied"));
    ASSERT_EQ("not satisfied", val.Unpack<std::string>());

    auto conf = ConfigPair("key", ConfigType<int32_t>(4));
    ASSERT_EQ(4, conf.UnpackValue<int32_t>());
    ASSERT_EQ("key", conf.GetKey().ToString());

    auto conf2 = ConfigPair("key", ConfigType<bool>(true));
    ASSERT_EQ(true, conf2.UnpackValue<bool>());

    auto conf3 = ConfigPair("key", ConfigType<std::string>("cheese"));
    ASSERT_EQ("cheese", conf3.UnpackValue<std::string>());
    ASSERT_EQ("cheese", conf3.GetValue().Unpack<std::string>());
    ASSERT_FALSE(conf3.GetValue().Is<int32_t>());
    ASSERT_TRUE(conf3.GetValue().Is<std::string>());

    auto nothing = ConfigValue();
    ASSERT_EQ(ConfigValue::None(), nothing);
    ASSERT_FALSE(nothing.Is<bool>());

    auto inttype1 = ConfigPair("key", ConfigType<int32_t>(4));
    auto inttype2 = ConfigPair("key", ConfigType<int32_t>(5));
    // They are the same type despite their different values
    ASSERT_TRUE(inttype1.GetValue().TypeCheck(inttype2.GetValue()));

    auto conf4 = ConfigPair("key", ConfigValue(ConfigType<std::string>("Many constructors")));
    ASSERT_EQ("Many constructors", conf4.UnpackValue<std::string>());
}

TEST(TestConfigTypes, TypeString)
{
    auto intval = ConfigValue(ConfigType<int32_t>(4));
    ASSERT_EQ("int32", intval.TypeToString());

    auto strval = ConfigValue(ConfigType<std::string>("string"));
    ASSERT_EQ("string", strval.TypeToString());

    auto boolval = ConfigValue(ConfigType<bool>(true));
    ASSERT_EQ("bool", boolval.TypeToString());

    auto intlistval = ConfigValue(ConfigType<std::vector<int32_t>>({4}));
    ASSERT_EQ("int32list", intlistval.TypeToString());

    auto strlistval = ConfigValue(ConfigType<std::vector<std::string>>({"string"}));
    ASSERT_EQ("stringlist", strlistval.TypeToString());

    auto boollistval = ConfigValue(ConfigType<std::vector<bool>>({true}));
    ASSERT_EQ("boollist", boollistval.TypeToString());

    auto nullval = ConfigValue();
    ASSERT_EQ("NULL", nullval.TypeToString());
}

TEST(TestConfigTypes, IsTesting)
{
    auto empty = ConfigValue();
    ASSERT_FALSE(empty.Is<std::string>());
    ASSERT_FALSE(empty.Is<bool>());
    ASSERT_FALSE(empty.Is<int32_t>());
    ASSERT_TRUE(empty.Empty());
}

TEST(TestConfigTypes, Vectors)
{
    auto val = ConfigValue(ConfigType<std::vector<bool>>({true, true, false}));
    ASSERT_EQ(std::vector<bool>({true, true, false}), val.Unpack<std::vector<bool>>());

    auto val2 = ConfigValue(ConfigType<std::vector<std::string>>({"cheese", "house", "music"}));
    ASSERT_EQ(std::vector<std::string>({"cheese", "house", "music"}),
              val2.Unpack<std::vector<std::string>>());

    auto val3 = ConfigValue(ConfigType<std::vector<int32_t>>({4, 5, 6}));
    ASSERT_EQ(std::vector<int32_t>({4, 5, 6}),
              val3.Unpack<std::vector<int32_t>>());
    ASSERT_NE(std::vector<int32_t>({6}),
              val3.Unpack<std::vector<int32_t>>());
}

TEST(TestConfigTypes, ConfigKeyBasics)
{
    auto key1 = ConfigKey("");
    ASSERT_EQ("", key1.ToString());
    ASSERT_EQ("", key1.Last().ToString());
    ASSERT_EQ("", key1.Init().ToString());

    auto key2 = ConfigKey("n1.n2.n3");
    ASSERT_EQ("n1.n2.n3", key2.ToString());
    ASSERT_EQ("n1.n2", key2.Init().ToString());
    ASSERT_EQ("n2.n3", key2.Tail().ToString());
    ASSERT_EQ("n3", key2.Last().ToString());
    ASSERT_EQ("n1", key2.Head().ToString());
    ASSERT_EQ(ConfigKey("n1.n2"), key2.Init());

    auto key8 = ConfigKey("");
    ASSERT_EQ(ConfigKey(""), key8.Init());
    ASSERT_EQ(ConfigKey(""), key8.Last());
    ASSERT_EQ(ConfigKey(""), key8.Head());
    ASSERT_EQ(ConfigKey(""), key8.Tail());

    auto key3 = ConfigKey("n1.n2.n3.n4.n5.*");
    ASSERT_TRUE(key3.IsWildcard());
    ASSERT_TRUE(key3.Last().IsWildcard());
    ASSERT_FALSE(key3.Init().IsWildcard());

    auto key4 = ConfigKey("n1.n2.n3.*");
    ASSERT_TRUE(key3.SubkeyMatches(key4));
    auto key5 = ConfigKey("n1.n2");
    ASSERT_TRUE(key3.SubkeyMatches(key5));
    auto key6 = ConfigKey("n1.n2.*");
    ASSERT_TRUE(key3.SubkeyMatches(key6));
    // This is a subsequence, but it doesn't match at the beginning so it doesn't match
    auto key7 = ConfigKey("n2.n3.n4.n5.*");
    ASSERT_FALSE(key3.SubkeyMatches(key7));

    ASSERT_TRUE(key3.SubkeyMatches(ConfigKey("n1.*.n3.*.n5")));
    ASSERT_TRUE(key3.SubkeyMatches(ConfigKey("n1.*.n3")));
    ASSERT_TRUE(key3.SubkeyMatches(ConfigKey("n1.*")));
    ASSERT_FALSE(key3.SubkeyMatches(ConfigKey("n1.*.:(.*")));
    ASSERT_FALSE(key2.SubkeyMatches(ConfigKey("n1.n2.n3.n4.n5.*")));
}

TEST(TestConfigTypes, UnwrapSinglePairTest)
{
    auto intpair = ConfigPair(ConfigKey("intval"), ConfigValue(ConfigType<int32_t>(4)));
    auto strpair = ConfigPair(ConfigKey("strval"), ConfigValue(ConfigType<std::string>("string")));
    auto boolpair = ConfigPair(ConfigKey("boolval"), ConfigValue(ConfigType<bool>(true)));

    ConfigPairMap intmap;
    intmap.emplace(intpair.GetKey(), intpair);
    ASSERT_EQ(4, UnwrapSinglePair(intmap).get().UnpackValue<int32_t>());

    ConfigPairMap strmap;
    strmap.emplace(strpair.GetKey(), strpair);
    ASSERT_EQ("string", UnwrapSinglePair(strmap).get().UnpackValue<std::string>());

    ConfigPairMap boolmap;
    boolmap.emplace(boolpair.GetKey(), boolpair);
    ASSERT_EQ(true, UnwrapSinglePair(boolmap).get().UnpackValue<bool>());

    ConfigPairMap empty;
    ASSERT_FALSE(UnwrapSinglePair(empty));
}
