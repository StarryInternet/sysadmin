
#include "CommitHistory.h"

#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <log4cxx/logger.h>
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/exceptions.h>

#include "LocalYAMLStorage.h"

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("CommitHistory"));
}

boost::optional<std::string> YAMLSerializer(const CommitHistory& commit)
{
    YAML::Node commitNode;
    commitNode["id"] = commit.mId;
    commitNode["parent-id"] = commit.mParent;
    commitNode["time"] = boost::posix_time::to_simple_string(commit.mTime);

    YAML::Node deltas;
    for (const auto& item : commit.mItems)
    {
        YAML::Node delta;
        delta["key"] = item.first.GetKey().ToString();
        YAML::Node old = UnpackIntoYAML(item.first);
        YAML::Node current = UnpackIntoYAML(item.second);
        delta["old"] = old;
        delta["current"] = current;
        deltas.push_back(delta);
    }

    commitNode["deltas"] = deltas;

    std::stringstream out;
    out << commitNode << std::endl;
    return boost::optional<std::string>(out.str());
}

boost::optional<CommitHistory> YAMLDeserializer(const std::string& buffer)
{
    YAML::Node commitNode = YAML::Load(buffer);
    CommitHistory commit;

    commit.mId = commitNode["id"].as<CommitHistory::CommitId>();
    commit.mParent = commitNode["parent-id"].as<CommitHistory::CommitId>();

    if (commitNode["time"])
    {
        std::string timeString = commitNode["time"].as<std::string>();
        try
        {
            commit.mTime = boost::posix_time::time_from_string(timeString);
        }
        catch(const boost::bad_lexical_cast&)
        {
            //This is the exception boost throws when it cannot convert a string to a posix_time::ptime
            commit.mTime = boost::posix_time::ptime(boost::posix_time::not_a_date_time);
        }
    }
    else
    {
        commit.mTime = boost::posix_time::ptime(boost::posix_time::not_a_date_time);
    }

    for (const auto& item : commitNode["deltas"])
    {
        std::string key = item["key"].as<std::string>();
        std::string oldtype = item["old"]["type"].as<std::string>();
        ConfigValue old = (oldtype != "NULL") ? ValueFromYAML(oldtype, item["old"]["value"]) : ConfigValue();
        std::string currenttype = item["current"]["type"].as<std::string>();
        ConfigValue current = (currenttype != "NULL") ? ValueFromYAML(currenttype, item["current"]["value"]) : ConfigValue();
        commit.mItems.emplace_back(ConfigPair(key, old), ConfigPair(key, current));
    }

    return boost::optional<CommitHistory>(commit);
}
