#pragma once

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <stdexcept>
#include <yaml-cpp/yaml.h>

namespace decibel
{
class LoadConfigError : public std::runtime_error
{
public:
    LoadConfigError(const std::string& str) : std::runtime_error(str)
    {
    }
};

template <typename T>
T LoadConfig(int argc, const char* argv[])
{
    if (argc <= 1)
    {
        throw LoadConfigError("Invalid number of arguments");
    }

    boost::filesystem::path configPath(argv[1]);

    if (!boost::filesystem::exists(configPath))
    {
        throw LoadConfigError("Path does not exist");
    }

    if (!boost::filesystem::is_regular_file(configPath))
    {
        throw LoadConfigError("Invalid file type");
    }

    auto yamlConfig = YAML::LoadFile(configPath.string());
    return yamlConfig.as<T>();
}

} // decibel
