
#pragma once

#include "ConfigTypes.h"
#include <yaml-cpp/yaml.h>

struct HookConfig
{
    HookConfig()
      : mKey("")
      , mOptional()
    {
    }

    HookConfig(const std::string& key, bool optional = false)
      : mKey(key)
      , mOptional(optional)
    {
    }

    ConfigPair::Key mKey;
    bool mOptional;
};

bool operator==(const HookConfig& lhs, const HookConfig& rhs);
bool operator!=(const HookConfig& lhs, const HookConfig& rhs);

// TODO move runlevel into this struct
struct HookOptions
{
    // TODO make this bit mask able so we can have multiple options
    enum RunOptions
    {
        DEFAULT,
        DETACH, // Detaches the hook from sysadmin, so it persists sysadmin's runtime
        IGNORE, // Ignores the error status of the hook
        INVALID
    };
};

HookOptions::RunOptions DecodeRunOptions(const std::string& optionstr);

struct TemplateConfig
{
    std::string mName;
    std::string mPath;
    std::string mDestination;
    std::vector<std::string> mArguments;
    std::vector<HookConfig> mHooks;
};

struct ServiceConfig
{

    std::string mName;
    std::string mPath;
    std::vector<HookConfig> mHooks;
    size_t mRunLevel;
    std::vector<std::string> mArguments;
    HookOptions::RunOptions mRunOptions;
};

struct FullHooksConfig
{
    std::vector<ServiceConfig> mServices;
    std::string mTemplaterPath;
    std::vector<TemplateConfig> mTemplates;
};

class HookConfigHasher
{
public:
    inline size_t operator()(const HookConfig& hookconfig) const
    {
        std::hash<std::string> hasher;
        return hasher(hookconfig.mKey.ToString());
    }
};


namespace YAML
{

template <>
struct convert<HookConfig>
{
    static Node encode(const HookConfig& config);

    static bool decode(const Node& node, HookConfig& config);
};

template <>
struct convert<TemplateConfig>
{
    static Node encode(const TemplateConfig& config);

    static bool decode(const Node& node, TemplateConfig& config);
};

template <>
struct convert<ServiceConfig>
{
    static Node encode(const ServiceConfig& config);

    static bool decode(const Node& node, ServiceConfig& config);
};

template <>
struct convert<FullHooksConfig>
{
    static Node encode(const FullHooksConfig& config);

    static bool decode(const Node& node, FullHooksConfig& config);
};

}
