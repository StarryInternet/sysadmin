
#include "LocalYAMLStorage.h"
#include "ConfigTypes.h"

#include <log4cxx/logger.h>
#include <cctype>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/filesystem.hpp>
#include <folly/File.h>
#include <folly/FileUtil.h>
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/exceptions.h>

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("LocalYAMLStorage"));
}

namespace dm = decibel::messaging;
namespace bfs = boost::filesystem;

namespace YAML
{

Node convert<ConfigType<std::vector<bool>>>::encode(const ConfigType<std::vector<bool>>& val)
{
    Node output;
    for (const auto& iter : val.mValue)
    {
        output.push_back(bool(iter));
    }
    return output;
}

}

YAML::Node LoadOrCreate(boost::filesystem::path path)
{
    YAML::Node config;
    if (bfs::exists(path))
    {
        config = YAML::LoadFile(path.string());
    }
    return config;
}

std::pair<bfs::path, std::string> TranslateKeyToPathPair(const ConfigPair::Key& key)
{
    std::vector<std::string> elems = key.KeyParts();
    bfs::path output;
    for (size_t i = 0; i < elems.size() - 1; i++)
    {
        output /= elems[i];
    }
    if (output.empty())
    {
        output /= bfs::path("global");
    }
    output.replace_extension(".yaml");
    return std::pair<bfs::path, std::string>(output, elems[elems.size() - 1]);
}

std::string TranslatePathToKey(const bfs::path& rootpath, bfs::path valuepath,
                               const std::string& bottomLevelKey)
{
    valuepath.replace_extension("");
    std::string rootStripped(valuepath.string());
    rootStripped.erase(rootStripped.find(rootpath.string()), rootpath.string().size());
    std::vector<std::string> elems;
    boost::split(elems, rootStripped, boost::is_any_of("/"));
    std::stringstream keybuilder;
    for (size_t i = 0; i < elems.size(); i++)
    {
        if (elems[i].size() > 0 && elems[i] != "global")
        {
            keybuilder << elems[i] << ".";
        }
    }
    keybuilder << bottomLevelKey;
    return keybuilder.str();
}

struct recursive_directory_range
{
    typedef bfs::recursive_directory_iterator iterator;
    recursive_directory_range(bfs::path p) : p_(p) {}

    iterator begin() { return bfs::recursive_directory_iterator(p_); }
    iterator end() { return bfs::recursive_directory_iterator(); }

    bfs::path p_;
};

ConfigValue ValueFromYAML(const std::string& type, const YAML::Node& node)
{
    if (type == "int32")
    {
        return ConfigValue(ConfigType<int32_t>(node.as<int32_t>()));
    }
    else if (type == "string")
    {
        return ConfigValue(ConfigType<std::string>(node.as<std::string>()));
    }
    else if (type == "bool")
    {
        return ConfigValue(ConfigType<bool>(node.as<bool>()));
    }
    else if (type == "int32list")
    {
        return ConfigValue(ConfigType<std::vector<int32_t>>(node.as<std::vector<int32_t>>()));
    }
    else if (type == "stringlist")
    {
        return ConfigValue(ConfigType<std::vector<std::string>>(node.as<std::vector<std::string>>()));
    }
    else if (type == "boollist")
    {
        return ConfigValue(ConfigType<std::vector<bool>>(node.as<std::vector<bool>>()));
    }
    else if (type == "NULL")
    {
        return ConfigValue();
    }

    LOG4CXX_ERROR(spLogger, "Encountered unknown type: " << type << ". Dropping value");
    return ConfigValue();
}

YAML::Node UnpackIntoYAML(const ConfigPair& value)
{
    YAML::Node valueNode;
    DYNAMIC_UNPACK(value.GetValue(), valueNode);

    YAML::Node output;
    output["type"] = value.GetValue().TypeToString();
    output["value"] = valueNode;

    return output;
}

LocalYAMLStorage::LocalYAMLStorage(const std::string& root)
  : mRoot(root)
  , mInFlightItems(100, this)
{
    bfs::create_directories(mRoot);
}

LocalYAMLStorage::LocalYAMLStorage(const bfs::path& root)
  : mRoot(root)
  , mInFlightItems(100, this)
{
    bfs::create_directories(mRoot);
}

LocalYAMLStorage::~LocalYAMLStorage()
{

}

IConfigurator::SetStatus LocalYAMLStorage::Set(const ConfigPair& value, dm::Protocol::Id id)
{
    mInFlightItems.Insert(id, value);
    return IConfigurator::SUCCESS;
}

ConfigPairMap LocalYAMLStorage::Get(const ConfigPair::Key& key) const
{
    ConfigPairMap output;
    if (key.IsWildcard())
    {
        auto subKeys = GetAllKeys(key);
        for (const auto& subkey : subKeys)
        {
            auto res = InternalGet(subkey);
            if (res)
            {
                output.emplace(subkey, res.get());
            }
        }
    }
    else
    {
        auto res = InternalGet(key);
        if (res)
        {
            output.emplace(key, res.get());
        }
    }

    return output;
}

boost::optional<ConfigPair> LocalYAMLStorage::InternalGet(const ConfigPair::Key& key) const
{
    auto paths = TranslateKeyToPathPair(key);
    auto rootpath = mRoot / paths.first;
    YAML::Node config = LoadOrCreate(rootpath);

    if (config.IsMap() && config[paths.second])
    {
        auto value = config[paths.second]["value"];
        // All types are stored as strings
        auto type = config[paths.second]["type"].as<std::string>();
        return boost::optional<ConfigPair>(ConfigPair(key, ValueFromYAML(type, value)));
    }
    return boost::none;
}

bool LocalYAMLStorage::TypeCheck(const ConfigPair& comp) const
{
    auto existing = UnwrapSinglePair(Get(comp.GetKey()));
    if (!existing)
    {
        return true;
    }
    return existing.get().TypeCheckValues(comp);
}

void LocalYAMLStorage::GroupSet(const ConfigPairList& items)
{
    std::map<bfs::path, std::list<std::pair<std::string, ConfigPair>>> buckets;
    // In order to avoid unnecessary file access, we gather all configpairs which will be
    // accessing the same file into one bucket so we can only read and write to it once
    for (auto item : items)
    {
        auto paths = TranslateKeyToPathPair(item.GetKey());
        if (!buckets.count(paths.first))
        {
            buckets.emplace(paths.first, std::list<std::pair<std::string, ConfigPair>>());
        }
        buckets[paths.first].push_back(std::pair<std::string, ConfigPair>(paths.second, item));
    }
    for (auto path : buckets)
    {
        auto rootpath = mRoot / path.first;
        YAML::Node config = LoadOrCreate(rootpath);

        for (auto item : path.second)
        {
            // TODO: This might be wrong, though the current behavior works. Basically,
            // when last.* keys are set initially they're all empty, and they shouldn't
            // be written to disc, but this if results in them being written. It's not a
            // problem that they're written to disc, since we handle them on the loading
            // side, but they should never be written in the first place
            if (item.second.GetValue().Empty() && config[item.first])
            {
                config.remove(item.first);
            }
            else
            {
                config[item.first] = UnpackIntoYAML(item.second);
            }
        }

        if (!bfs::exists(rootpath.parent_path()))
        {
            bfs::create_directories(rootpath.parent_path());
        }
        auto fout = folly::File(rootpath.string(), O_WRONLY | O_CREAT | O_TRUNC);
        if (fout)
        {
            YAML::Emitter emitter;
            emitter << config;
            std::string out_buffer(emitter.c_str(), emitter.c_str() + emitter.size());
            folly::writeFull(fout.fd(), out_buffer.c_str(), out_buffer.size());
            folly::fsyncNoInt(fout.fd());
        }
        else
        {
            LOG4CXX_ERROR(spLogger, "Couldn't open file for writing: " << rootpath.string());
        }
        if (!fout.closeNoThrow())
        {
            LOG4CXX_ERROR(spLogger, "Couldn't close file: " << rootpath.string());
        }
    }
}

ConfigPairMap LocalYAMLStorage::GetAll() const
{
    ConfigPairMap output;
    for (const auto& file : recursive_directory_range(mRoot))
    {
        if (bfs::is_regular_file(file))
        {
            YAML::Node config = YAML::LoadFile(file.path().string());
            for (const auto node : config)
            {
                std::string key = TranslatePathToKey(mRoot, file.path(), node.first.as<std::string>());
                YAML::Node valueNode = node.second["value"];
                ConfigValue value = ValueFromYAML(node.second["type"].as<std::string>(), valueNode);
                if (!value.Empty())
                {
                    output.emplace(key, ConfigPair(key, value));
                }
            }
        }
    }

    return output;
}

std::vector<ConfigPair::Key> LocalYAMLStorage::GetAllKeys(const ConfigPair::Key& ns) const
{
    // TODO this could be more efficient if it just loaded files under the namespace
    BOOST_ASSERT(ns.IsWildcard());
    std::vector<ConfigPair::Key> output;
    for (const auto& file : recursive_directory_range(mRoot))
    {
        if (bfs::is_regular_file(file))
        {
            YAML::Node config = YAML::LoadFile(file.path().string());
            for (const auto node : config)
            {
                auto key = ConfigPair::Key(TranslatePathToKey(mRoot, file.path(), node.first.as<std::string>()));
                if (key.SubkeyMatches(ns))
                {
                    output.push_back(key);
                }
            }
        }
    }

    return output;
}

folly::Future<CommitHistory::CommitId>
LocalYAMLStorage::Commit(size_t id,
                         sysadminctl::CommitConfig )
{
    GroupSet(mInFlightItems.GetBucket(id).get());
    return folly::makeFuture(CommitHistory::CommitId(0));
}

void LocalYAMLStorage::Drop(size_t id)
{
    mInFlightItems.Drop(id);
}

void LocalYAMLStorage::Erase(const ConfigPair::Key& key, size_t id)
{
    LOG4CXX_WARN(spLogger, "Calling this method is very inefficient");
    mInFlightItems.Erase(id, key);
}

ConfigPairList LocalYAMLStorage::InFlightItems(size_t id)
{
    auto maybebucket = mInFlightItems.GetBucket(id);
    if (!maybebucket)
    {
        return ConfigPairList();
    }

    return maybebucket.get();
}
