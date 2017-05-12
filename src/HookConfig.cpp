
#include "HookConfig.h"

#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("FullHooksConfig"));
}

#define DECODE_REQUIRED(NODE, TARGET, KEY, TYPE)                 \
    if ((NODE)[KEY])                                             \
    {                                                            \
        (TARGET) = (NODE)[KEY].as<TYPE>();                       \
    }                                                            \
    else                                                         \
    {                                                            \
        LOG4CXX_WARN(spLogger, "Could not decode key: " << KEY); \
        return false;                                            \
    }

#define DECODE_OPTIONAL(NODE, TARGET, KEY, TYPE, DEFAULT) \
    if ((NODE)[KEY]) \
    { \
        (TARGET) = (NODE)[KEY].as<TYPE>(); \
    } \
    else \
    { \
        (TARGET) = (DEFAULT); \
    }

#define DECODE_REQUIRED_SEQUENCE(NODE, TARGET, KEY, TYPE) \
    if ((NODE)[KEY] && (NODE)[KEY].IsSequence())          \
    {                                                     \
        (TARGET) = (NODE)[KEY].as<TYPE>();                \
    }                                                     \
    else                                                  \
    {                                                     \
        return false;                                     \
    }

#define DECODE_OPTIONAL_SEQUENCE(NODE, TARGET, KEY, TYPE) \
    if ((NODE)[KEY] && (NODE)[KEY].IsSequence())          \
    {                                                     \
        (TARGET) = (NODE)[KEY].as<TYPE>();                \
    }

bool operator==(const HookConfig& lhs, const HookConfig& rhs)
{
    return lhs.mKey == rhs.mKey && lhs.mOptional == rhs.mOptional;
}

bool operator!=(const HookConfig& lhs, const HookConfig& rhs)
{
    return (lhs == rhs);
}

namespace YAML
{

Node convert<HookConfig>::encode(const HookConfig& )
{
    return Node();
}

bool convert<HookConfig>::decode(const Node& node, HookConfig& config)
{
    try
    {
        config.mKey = ConfigPair::Key(node.as<std::string>());
        config.mOptional = false;
        return true;
    }
    catch (const YAML::Exception&)
    {
        for (const auto& ci : node)
        {
            config.mKey = ConfigPair::Key(ci.first.as<std::string>());
            config.mOptional = true;
            return true;
        }
    }

    return false;
}

Node convert<TemplateConfig>::encode(const TemplateConfig& )
{
    return Node();
}

bool convert<TemplateConfig>::decode(const Node& node, TemplateConfig& config)
{
    DECODE_REQUIRED(node, config.mName, "name", std::string);
    DECODE_REQUIRED(node, config.mPath, "template_location", std::string);
    DECODE_REQUIRED(node, config.mDestination, "destination", std::string);
    DECODE_REQUIRED_SEQUENCE(node, config.mHooks, "hooks", std::vector<HookConfig>);
    DECODE_OPTIONAL_SEQUENCE(node, config.mArguments, "arguments", std::vector<std::string>);

    return true;
}

Node convert<ServiceConfig>::encode(const ServiceConfig& )
{
    return Node();
}

HookOptions::RunOptions DecodeRunOptions(const std::string& optionstr)
{
    if (optionstr == "detach")
    {
        return HookOptions::RunOptions::DETACH;
    }
    if (optionstr == "default")
    {
        return HookOptions::RunOptions::DEFAULT;
    }
    if (optionstr == "ignore")
    {
        return HookOptions::RunOptions::IGNORE;
    }
    return HookOptions::RunOptions::INVALID;
}

bool convert<ServiceConfig>::decode(const Node& node, ServiceConfig& config)
{
    DECODE_REQUIRED(node, config.mName, "name", std::string);
    DECODE_REQUIRED(node, config.mPath, "script", std::string);
    DECODE_REQUIRED_SEQUENCE(node, config.mHooks, "hooks", std::vector<HookConfig>);
    DECODE_OPTIONAL(node, config.mRunLevel, "runlevel", size_t, 1);
    DECODE_OPTIONAL_SEQUENCE(node, config.mArguments, "arguments", std::vector<std::string>);

    if (node["options"])
    {
        config.mRunOptions = DecodeRunOptions(node["options"].as<std::string>());
        if (config.mRunOptions == HookOptions::RunOptions::INVALID)
        {
            return false;
        }
    }
    else
    {
        config.mRunOptions = DecodeRunOptions("default");
    }

    return true;
}

Node convert<FullHooksConfig>::encode(const FullHooksConfig& )
{
    return Node();
}

bool convert<FullHooksConfig>::decode(const Node& node, FullHooksConfig& config)
{
    DECODE_OPTIONAL_SEQUENCE(node, config.mServices, "services", std::vector<ServiceConfig>);
    DECODE_OPTIONAL_SEQUENCE(node, config.mTemplates, "templates", std::vector<TemplateConfig>);
    DECODE_REQUIRED(node, config.mTemplaterPath, "templater_path", std::string);

    return true;
}

}
