
#pragma once

#include "IConfigurator.h"
#include "ICommandInterface.h"
#include "ConfigTypes.h"
#include "LockedConfigPairMap.h"

class PassThroughConfigurator : public IConfigurator
{
public:
    PassThroughConfigurator(IConfigurator* storage);

    virtual ~PassThroughConfigurator();

public: // IConfigurator
    virtual IConfigurator::SetStatus Set(const ConfigPair& item,
                                         IConfigurator::ClientId clientId);

    virtual ConfigPairMap Get(const ConfigPair::Key& key) const;

    virtual ConfigPairMap GetAll() const;

    std::vector<ConfigPair::Key> GetAllKeys(const ConfigPair::Key& ns) const;

    virtual bool TypeCheck(const ConfigPair& pair) const;

    virtual folly::Future<CommitHistory::CommitId>
    Commit(IConfigurator::ClientId clientId,
           sysadminctl::CommitConfig config_opt = sysadminctl::CommitConfig::DEFAULT);

    virtual void Drop(IConfigurator::ClientId clientId);

    virtual void Erase(const ConfigPair::Key& key, IConfigurator::ClientId clientId);

    ConfigPairList InFlightItems(IConfigurator::ClientId clientId);

private:
    IConfigurator* mBackingStore;
};
