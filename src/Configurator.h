
#pragma once

#include "IConfigurator.h"
#include "ICommandInterface.h"
#include "IHistoryStorage.h"
#include "ConfigTypes.h"
#include "LockedConfigPairMap.h"
#include "TransactionManager.h"

#include <list>

class CommitError : public std::runtime_error {
public:
    CommitError(const std::string& str, size_t failed_commit_id)
      : std::runtime_error(str)
      , mCommitId(failed_commit_id)
    {}

    size_t mCommitId;
};

class Configurator : public ICommandInterface, public IConfigurator
{
public:
    Configurator(IConfigurator* storage, IHistoryStorage* history, size_t maxTransactions = 100);

    virtual ~Configurator();

public: // ICommandInterface
    virtual void RegisterCommitHook(ICommandInterface::CommitHook hook);

    folly::Future<folly::Unit> Rollback(CommitHistory::CommitId id);

    std::vector<BlameDelta> Blame(const ConfigPair::Key& key) { return mCommitHistory.Blame(key); }

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

    void Erase(const ConfigPair::Key& key, IConfigurator::ClientId clientId);

    ConfigPairList InFlightItems(IConfigurator::ClientId clientId);

private:
    void StoreInFlightItem(const ConfigPair& item, IConfigurator::ClientId clientId);

    ConfigPairList PruneUnchangedValues(IConfigurator::ClientId clientId);

    void SaveLastItems(IConfigurator::ClientId clientId);

private:
    IConfigurator* mSystemStore;
    TransactionManager mCommitHistory;
    LockedConfigPairMap mInFlightItems;
    ICommandInterface::CommitHook mServiceHooksCallback;
};
