
#pragma once

#include "ConfigTypes.h"

#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

struct CommitHistory
{
    typedef size_t CommitId;
    typedef std::pair<ConfigPair, ConfigPair> ConfigDelta;
    typedef std::vector<ConfigDelta> ConfigDeltaList;

    CommitId mId;
    CommitId mParent;
    ConfigDeltaList mItems;
    boost::posix_time::ptime mTime;
};

typedef std::function<boost::optional<std::string>(const CommitHistory& commit)> CommitSerializer;
typedef std::function<boost::optional<CommitHistory>(const std::string& buffer)> CommitDeserializer;

boost::optional<std::string> YAMLSerializer(const CommitHistory& commit);

boost::optional<CommitHistory> YAMLDeserializer(const std::string& buffer);
