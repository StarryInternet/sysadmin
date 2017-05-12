#pragma once

#include <yaml-cpp/yaml.h>

struct SysAdminConfig
{
    std::string host;
    uint16_t port;
    std::string log4cxxConfig;
    std::string storagePath;
    std::string hookConfig;
    std::string historyStorage;
    size_t parallelHooks;
};

namespace YAML
{

template <>
struct convert<SysAdminConfig>
{
    static Node encode(const SysAdminConfig& config);

    static bool decode(const Node& node, SysAdminConfig& config);
};

} // YAML
