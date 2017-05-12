#include <log4cxx/logger.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "HookConfig.h"
#include "yaml-cpp/yaml.h"

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_hook_config"));
}

#define REQUIRE_EQUAL_VECTORS(a, b) EXPECT_THAT((a), ::testing::ContainerEq(b));

TEST(Config, Basic)
{
    YAML::Node node = YAML::Load(
        "templater_path: /path/to/templater.py\n"
        "templates:\n"
        "    [\n"
        "        {\n"
        "            name: test.conf,\n"
        "            template_location: /test.tmpl,\n"
        "            destination: /etc/test.conf,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip,\n"
        "                    endip\n"
        "                ],\n"
        "            arguments:\n"
        "                [\n"
        "                    arg1,\n"
        "                    arg2\n"
        "                ]\n"
        "        }\n"
        "    ]\n"
        "\n"
        "services:\n"
        "    [\n"
        "        {\n"
        "            name: dnsmasq,\n"
        "            script: /restarter,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip,\n"
        "                    endip\n"
        "                ],\n"
        "            arguments:\n"
        "                [\n"
        "                    -p 53\n"
        "                ]\n"
        "        }\n"
        "    ]");
    FullHooksConfig config = node.as<FullHooksConfig>();
    ASSERT_EQ("/path/to/templater.py", config.mTemplaterPath);
    ASSERT_EQ("test.conf", config.mTemplates[0].mName);
    ASSERT_EQ("/test.tmpl", config.mTemplates[0].mPath);
    ASSERT_EQ("/etc/test.conf", config.mTemplates[0].mDestination);
    REQUIRE_EQUAL_VECTORS(std::vector<HookConfig>({HookConfig("startip"), HookConfig("endip")}),
                          config.mTemplates[0].mHooks);
    REQUIRE_EQUAL_VECTORS(std::vector<std::string>({"arg1", "arg2"}),
                                                config.mTemplates[0].mArguments);

    ASSERT_EQ("dnsmasq", config.mServices[0].mName);
    ASSERT_EQ("/restarter", config.mServices[0].mPath);
    ASSERT_EQ(HookOptions::RunOptions::DEFAULT, config.mServices[0].mRunOptions);
    REQUIRE_EQUAL_VECTORS(std::vector<HookConfig>({HookConfig("startip"), HookConfig("endip")}),
                          config.mServices[0].mHooks);
    REQUIRE_EQUAL_VECTORS(std::vector<std::string>({"-p 53"}),
                          config.mServices[0].mArguments);
}

TEST(Config, NoTemplates)
{
    YAML::Node node = YAML::Load(
        "templater_path: /path/to/templater.py\n"
        "services:\n"
        "    [\n"
        "        {\n"
        "            name: dnsmasq,\n"
        "            script: /restarter,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip,\n"
        "                    endip\n"
        "                ]\n"
        "        }\n"
        "    ]");
    FullHooksConfig config = node.as<FullHooksConfig>();
    ASSERT_EQ("/path/to/templater.py", config.mTemplaterPath);
    ASSERT_EQ("dnsmasq", config.mServices[0].mName);
    ASSERT_EQ("/restarter", config.mServices[0].mPath);
    REQUIRE_EQUAL_VECTORS(std::vector<HookConfig>({HookConfig("startip"), HookConfig("endip")}),
                          config.mServices[0].mHooks);
}

TEST(Config, NoServices)
{
    YAML::Node node = YAML::Load(
        "templater_path: /path/to/templater.py\n"
        "templates:\n"
        "    [\n"
        "        {\n"
        "            name: test.conf,\n"
        "            template_location: /test.tmpl,\n"
        "            destination: /etc/test.conf,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip,\n"
        "                    endip\n"
        "                ]\n"
        "        }\n"
        "    ]");
    FullHooksConfig config = node.as<FullHooksConfig>();
    ASSERT_EQ("/path/to/templater.py", config.mTemplaterPath);
    ASSERT_EQ("test.conf", config.mTemplates[0].mName);
    ASSERT_EQ("/test.tmpl", config.mTemplates[0].mPath);
    ASSERT_EQ("/etc/test.conf", config.mTemplates[0].mDestination);
    REQUIRE_EQUAL_VECTORS(std::vector<HookConfig>({HookConfig("startip"), HookConfig("endip")}),
                          config.mTemplates[0].mHooks);
}

TEST(Config, OptionalHooks)
{
    YAML::Node node = YAML::Load(
        "templater_path: /path/to/templater.py\n"
        "templates:\n"
        "    [\n"
        "        {\n"
        "            name: test.conf,\n"
        "            template_location: /test.tmpl,\n"
        "            destination: /etc/test.conf,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip: optional,\n"
        "                    endip\n"
        "                ]\n"
        "        }\n"
        "    ]\n"
        "\n"
        "services:\n"
        "    [\n"
        "        {\n"
        "            name: dnsmasq,\n"
        "            script: /restarter,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip,\n"
        "                    endip: optional\n"
        "                ]\n"
        "        }\n"
        "    ]");
    FullHooksConfig config = node.as<FullHooksConfig>();
    ASSERT_EQ("/path/to/templater.py", config.mTemplaterPath);
    ASSERT_EQ("test.conf", config.mTemplates[0].mName);
    ASSERT_EQ("/test.tmpl", config.mTemplates[0].mPath);
    ASSERT_EQ("/etc/test.conf", config.mTemplates[0].mDestination);
    REQUIRE_EQUAL_VECTORS(std::vector<HookConfig>({HookConfig("startip", true),
                                                   HookConfig("endip", false)}),
                          config.mTemplates[0].mHooks);

    ASSERT_EQ("dnsmasq", config.mServices[0].mName);
    ASSERT_EQ("/restarter", config.mServices[0].mPath);
    REQUIRE_EQUAL_VECTORS(std::vector<HookConfig>({HookConfig("startip", false),
                                                   HookConfig("endip", true)}),
                          config.mServices[0].mHooks);
}

TEST(Config, OptionalArguments)
{
    YAML::Node node = YAML::Load(
        "templater_path: /path/to/templater.py\n"
        "templates:\n"
        "    [\n"
        "        {\n"
        "            name: test.conf,\n"
        "            template_location: /test.tmpl,\n"
        "            destination: /etc/test.conf,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip: optional,\n"
        "                    endip\n"
        "                ]\n"
        "        }\n"
        "    ]\n"
        "\n"
        "services:\n"
        "    [\n"
        "        {\n"
        "            name: dnsmasq,\n"
        "            script: /restarter,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip,\n"
        "                    endip\n"
        "                ],\n"
        "            arguments:\n"
        "                [\n"
        "                    -p 53\n"
        "                ]\n"
        "        }\n"
        "    ]");
    FullHooksConfig config = node.as<FullHooksConfig>();
    REQUIRE_EQUAL_VECTORS(std::vector<std::string>({"-p 53"}),
                          config.mServices[0].mArguments);
}

TEST(Config, RunOptions)
{
    YAML::Node node = YAML::Load(
        "templater_path: /path/to/templater.py\n"
        "templates:\n"
        "    [\n"
        "        {\n"
        "            name: test.conf,\n"
        "            template_location: /test.tmpl,\n"
        "            destination: /etc/test.conf,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip: optional,\n"
        "                    endip\n"
        "                ]\n"
        "        }\n"
        "    ]\n"
        "\n"
        "services:\n"
        "    [\n"
        "        {\n"
        "            name: dnsmasq,\n"
        "            script: /restarter,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip,\n"
        "                    endip\n"
        "                ],\n"
        "            arguments:\n"
        "                [\n"
        "                    -p 53\n"
        "                ],\n"
        "            options: detach\n"
        "        }\n"
        "    ]");
    FullHooksConfig config = node.as<FullHooksConfig>();
    REQUIRE_EQUAL_VECTORS(std::vector<std::string>({"-p 53"}),
                          config.mServices[0].mArguments);
    ASSERT_EQ(HookOptions::RunOptions::DETACH, config.mServices[0].mRunOptions);
}

TEST(Config, IgnoreRunOption)
{
    YAML::Node node = YAML::Load(
        "templater_path: /path/to/templater.py\n"
        "templates:\n"
        "    [\n"
        "        {\n"
        "            name: test.conf,\n"
        "            template_location: /test.tmpl,\n"
        "            destination: /etc/test.conf,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip: optional,\n"
        "                    endip\n"
        "                ]\n"
        "        }\n"
        "    ]\n"
        "\n"
        "services:\n"
        "    [\n"
        "        {\n"
        "            name: dnsmasq,\n"
        "            script: /restarter,\n"
        "            hooks:\n"
        "                [\n"
        "                    startip,\n"
        "                    endip\n"
        "                ],\n"
        "            arguments:\n"
        "                [\n"
        "                    -p 53\n"
        "                ],\n"
        "            options: ignore\n"
        "        }\n"
        "    ]");
    FullHooksConfig config = node.as<FullHooksConfig>();
    REQUIRE_EQUAL_VECTORS(std::vector<std::string>({"-p 53"}),
                          config.mServices[0].mArguments);
    ASSERT_EQ(HookOptions::RunOptions::IGNORE, config.mServices[0].mRunOptions);
}
