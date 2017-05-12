
#pragma once

#include "ConfigTypes.h"
#include "CommitHistory.h"

#include "decibel/messaging/Protocol.h"

#include "sysadmin-api/sysadminctl.pb.h"

#include <list>
#include <map>
#include <boost/optional.hpp>

class IConfigurator
{
public:
    enum SetStatus
    {
        SUCCESS,
        TYPE_MISMATCH,
        LOCKED_QUEUE,
        SUCCESS_NEW_KEY
    };

public:

    typedef decibel::messaging::Protocol::Id ClientId;

public:
    virtual SetStatus Set(const ConfigPair& item, decibel::messaging::Protocol::Id clientId) = 0;

    virtual ConfigPairMap Get(const ConfigPair::Key& key) const = 0;

    virtual ConfigPairMap GetAll() const = 0;

    virtual std::vector<ConfigPair::Key> GetAllKeys(const ConfigPair::Key& nspace) const = 0;

    virtual bool TypeCheck(const ConfigPair& pair) const = 0;

    virtual folly::Future<CommitHistory::CommitId>
    Commit(ClientId clientId,
           sysadminctl::CommitConfig config_opt = sysadminctl::CommitConfig::DEFAULT) = 0;

    virtual void Drop(ClientId clientId) = 0;

    virtual void Erase(const ConfigPair::Key& key, ClientId clientId) = 0;

    virtual ConfigPairList InFlightItems(ClientId clientId) = 0;
};
