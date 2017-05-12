#include <gtest/gtest.h>

#include "decibel/Config.h"

#include <yaml-cpp/yaml.h>

#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_config"));
}

using namespace decibel;

#define YAML_MAYBE_UPDATE(NODE, FIELD, TYPE, TARGET)                           \
    if (NODE[FIELD])                                                           \
    {                                                                          \
        TARGET = NODE[FIELD].as<TYPE>(TARGET);                                 \
    }

struct MockConfig
{
    int x;
    bool y;
    std::string z;
};

namespace YAML
{
template <>
struct convert<MockConfig>
{
    static Node encode(const MockConfig&)
    {
        Node node;
        return node;
    }

    static bool decode(const Node& node, MockConfig& config)
    {
        LOG4CXX_INFO(spLogger, "Decoding config");
        YAML_MAYBE_UPDATE(node, "x", int, config.x);
        YAML_MAYBE_UPDATE(node, "y", bool, config.y);
        YAML_MAYBE_UPDATE(node, "z", std::string, config.z);
        return true;
    }
};

} // YAML

TEST(LoadConfigTest, NoArgv)
{
    EXPECT_THROW(LoadConfig<MockConfig>(0, nullptr), LoadConfigError);
    EXPECT_THROW(LoadConfig<MockConfig>(1, nullptr), LoadConfigError);
}

TEST(LoadConfigTest, PathDoesntExist)
{
    char const* argv[] = {"decibel", "/this/is/not/a/path", NULL};
    EXPECT_THROW(LoadConfig<MockConfig>(2, argv), LoadConfigError);
}

TEST(LoadConfigTest, PathIsAFile)
{
    boost::filesystem::path path("../../test/testdata/config.d/config.yaml");
    char const* argv[] = {"decibel", path.c_str(), NULL};
    auto config = LoadConfig<MockConfig>(2, argv);
    EXPECT_EQ(5, config.x);
    EXPECT_EQ(false, config.y);
    EXPECT_EQ("helloworld", config.z);
}

// TODO
// TEST(LoadConfigTest, PathIsADirectory)
// {
//     boost::filesystem::path path("test/testdata/config.d/");
//     char const *argv[] = {"decibel", path.c_str(), NULL};
//     auto config = LoadConfig<MockConfig>(2, argv);
//     EXPECT_EQ(5, config.x);
//     EXPECT_EQ(false, config.y);
//     EXPECT_EQ("helloworld", config.z);
// }

TEST(LoadConfigTest, MissingKey)
{
    boost::filesystem::path path("../../test/testdata/config.d/00_config.yaml");
    char const* argv[] = {"decibel", path.c_str(), NULL};
    auto config = LoadConfig<MockConfig>(2, argv);
    EXPECT_EQ(5, config.x);
    EXPECT_EQ(true, config.y);
    EXPECT_EQ("", config.z);
}
