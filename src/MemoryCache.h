
#pragma once

#include "CommitHistory.h"
#include "ConfigTypes.h"
#include "IConfigurator.h"
#include "LockedConfigPairMap.h"

#include "sysadmin-api/sysadminctl.pb.h"

#include <unordered_map>

class MemoryCache : public IConfigurator
{
public:
    struct DirtyConfigPair
    {
        DirtyConfigPair(const ConfigPair& data)
          : mData(data)
          , mDirty(false)
        {
        }

        ConfigPair mData;
        bool mDirty;
    };

public:
    MemoryCache(IConfigurator* backingStore);

public: // IConfigurator
    IConfigurator::SetStatus Set(const ConfigPair& item, decibel::messaging::Protocol::Id clientId);

    ConfigPairMap Get(const ConfigPair::Key& key) const;

    bool TypeCheck(const ConfigPair& pair) const;

    ConfigPairMap GetAll() const;

    std::vector<ConfigPair::Key> GetAllKeys(const ConfigPair::Key& ns) const;

    // ICommandInterface methods moving into IConfigurator
    folly::Future<CommitHistory::CommitId>
    Commit(size_t id,
           sysadminctl::CommitConfig config_opt = sysadminctl::CommitConfig::DEFAULT);

    void Drop(size_t id);

    void Erase(const ConfigPair::Key& key, size_t id);

    ConfigPairList InFlightItems(size_t id);

private:
    boost::optional<ConfigPair> InternalGet(const ConfigPair::Key& key) const;

    void UpdateItems(const ConfigPairList& items);

private:
    IConfigurator* mBackingStore;
    std::unordered_map<ConfigPair::Key, DirtyConfigPair, ConfigKeyHasher> mValues;
    LockedConfigPairMap mInFlightItems;
};
