
#pragma once

#include <boost/optional.hpp>

#include "sysadmin-api/sysadminctl.pb.h"

#include "ConfigTypes.h"
#include "CommitHistory.h"
#include "ICommandInterface.h"

class ProtobufTranslator
{
public:
    ProtobufTranslator();

    ~ProtobufTranslator();

    void CopyConfigValue(const ConfigValue& from, sysadminctl::ConfigValue* to);

    boost::optional<ConfigPair::Key> TranslateToGet(const sysadminctl::Command& message);

    boost::optional<ConfigPair::Key> TranslateToErase(const sysadminctl::Command& message);

    boost::optional<ConfigPair> TranslateToSet(const sysadminctl::Command& message);

    boost::optional<ConfigPair::Key> TranslateToBlame(const sysadminctl::Command& message);

    sysadminctl::Response TranslateFromGet(const std::vector<ConfigPair>& key);

    boost::optional<CommitHistory::CommitId> TranslateToRollback(const sysadminctl::Command& message);

    sysadminctl::Response TranslateFromBlame(const ConfigPair::Key& key, const std::vector<BlameDelta>& commits);

    sysadminctl::Response TranslateFromInFlight(const std::vector<ConfigPair>& items);
};
