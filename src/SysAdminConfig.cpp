#include "SysAdminConfig.h"

namespace YAML
{

Node convert<SysAdminConfig>::encode(const SysAdminConfig&)
{
    return Node();
}

bool convert<SysAdminConfig>::decode(const Node& node, SysAdminConfig& config)
{
    if (!node["port"] || !node["log4cxx-config"] || !node["storage-path"] || !node["history-storage"])
    {
        return false;
    }
    if (node["host"])
    {
        config.host = node["host"].as<std::string>();
    }
    else
    {
        config.host = "127.0.0.1";
    }
    config.port = node["port"].as<uint16_t>();
    config.log4cxxConfig = node["log4cxx-config"].as<std::string>();
    config.storagePath = node["storage-path"].as<std::string>();
    config.historyStorage = node["history-storage"].as<std::string>();
    if (node["hook-config"])
    {
        config.hookConfig = node["hook-config"].as<std::string>();
    }
    else
    {
        config.hookConfig = "";
    }

    if (node["parallel-hooks"])
    {
        config.parallelHooks = node["parallel-hooks"].as<size_t>();
    }
    else
    {
        config.parallelHooks = 32;
    }

    return true;
}

}
