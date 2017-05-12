
#include "Configurator.h"
#include "ExternalRunner.h"

#include <boost/assert.hpp>
#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("Configurator"));
}

Configurator::Configurator(IConfigurator* storage, IHistoryStorage* history,
                           size_t maxTransactions)
  : mSystemStore(storage)
  , mCommitHistory(this, history, 100)
  , mInFlightItems(maxTransactions, this)
  , mServiceHooksCallback()
{
    BOOST_ASSERT(storage);
    mServiceHooksCallback = [](const ConfigPairList&, bool)
    {
        return folly::makeFuture();
    };
}

Configurator::~Configurator()
{
}

folly::Future<CommitHistory::CommitId>
Configurator::Commit(IConfigurator::ClientId clientId,
                     sysadminctl::CommitConfig config_opt)
{
    if (mInFlightItems.Exists(clientId) && !mInFlightItems.IsBucketLocked(clientId))
    {
        mInFlightItems.LockBucket(clientId);
        auto changed = PruneUnchangedValues(clientId);
        CommitHistory::CommitId commitId = 0;
        if (changed.size() > 0)
        {
            commitId = mCommitHistory.LogCommit(changed);
        }
        SaveLastItems(clientId);
        auto toCommit = mInFlightItems.GetBucket(clientId).get();
        for (const auto& item : toCommit)
        {
            mSystemStore->Set(item, clientId);
        }
        mSystemStore->Commit(clientId);
        auto run_services = config_opt == sysadminctl::CommitConfig::DEFAULT;
        if (run_services || config_opt == sysadminctl::CommitConfig::TEMPLATE_ONLY)
        {
            return mServiceHooksCallback(changed,
                                         run_services).then([this, clientId, commitId]()
            {
                LOG4CXX_INFO(spLogger, "All commit signals completed successfully. XID: " << clientId);
                this->Drop(clientId);
                return commitId;
            }).onError([this, clientId, commitId](const ExternalRunnerError& err) -> CommitHistory::CommitId
            {
                LOG4CXX_ERROR(spLogger, "A commit signal failed, Re-attempt the commit: " <<
                              err.what() << ". XID: " << clientId);
                this->Drop(clientId);
                throw CommitError(err.what(), commitId);
            });
        }
        LOG4CXX_INFO(spLogger, "No commit signals needed. Completed successfully. XID: " <<
                     clientId);
        this->Drop(clientId);
        return folly::makeFuture(commitId);
    }
    CommitHistory::CommitId id = 0;
    return folly::makeFuture(id);
}

void Configurator::Drop(IConfigurator::ClientId clientId)
{
    mInFlightItems.Drop(clientId);
}

IConfigurator::SetStatus Configurator::Set(const ConfigPair& item, IConfigurator::ClientId clientId)
{
    bool matches = mSystemStore->TypeCheck(item);
    auto status = IConfigurator::SUCCESS;

    if (!matches)
    {
        return IConfigurator::TYPE_MISMATCH;
    }
    if (mInFlightItems.IsBucketLocked(clientId))
    {
        return IConfigurator::LOCKED_QUEUE;
    }
    if (!UnwrapSinglePair(Get(item.GetKey())))
    {
        status = IConfigurator::SUCCESS_NEW_KEY;
    }
    mInFlightItems.Insert(clientId, item);
    return status;
}

ConfigPairMap Configurator::Get(const ConfigPair::Key& key) const
{
    return mSystemStore->Get(key);
}

void Configurator::RegisterCommitHook(ICommandInterface::CommitHook hook)
{
    mServiceHooksCallback = hook;
}

ConfigPairMap Configurator::GetAll() const
{
    return mSystemStore->GetAll();
}

std::vector<ConfigPair::Key> Configurator::GetAllKeys(const ConfigPair::Key& ns) const
{
    BOOST_ASSERT(ns.IsWildcard());
    return mSystemStore->GetAllKeys(ns);
}

bool Configurator::TypeCheck(const ConfigPair& pair) const
{
    return mSystemStore->TypeCheck(pair);
}

void Configurator::Erase(const ConfigPair::Key& key, IConfigurator::ClientId clientId)
{
    mInFlightItems.Erase(clientId, key);
}

ConfigPairList Configurator::InFlightItems(IConfigurator::ClientId clientId)
{
    auto maybebucket = mInFlightItems.GetBucket(clientId);
    if (!maybebucket)
    {
        return ConfigPairList();
    }

    return maybebucket.get();
}

folly::Future<folly::Unit>
Configurator::Rollback(CommitHistory::CommitId id)
{
    return mCommitHistory.Rollback(id);
}

ConfigPairList Configurator::PruneUnchangedValues(IConfigurator::ClientId clientId)
{
    ConfigPairList pruned;

    auto maybeItems = mInFlightItems.GetBucket(clientId);
    if (!maybeItems)
    {
        return pruned;
    }

    std::unordered_map<ConfigPair::Key, ConfigPair, ConfigKeyHasher> merged;
    auto items = maybeItems.get();
    for (const auto& item : items)
    {
        // Always prune last.* keys cause they can't be hook'd on
        if (item.GetKey().SubkeyMatches(ConfigKey("last.*")))
        {
            continue;
        }
        if (merged.count(item.GetKey()))
        {
            merged.erase(item.GetKey());
        }
        merged.emplace(item.GetKey(), item);
    }

    for (const auto& item : merged)
    {
        auto last_key = ConfigKey(item.first.ToString());
        auto old_item = UnwrapSinglePair(mSystemStore->Get(last_key));
        if (!old_item || old_item.get().GetValue() != item.second.GetValue())
        {
            pruned.push_back(item.second);
        }
    }
    return pruned;
}

void Configurator::SaveLastItems(IConfigurator::ClientId clientId)
{
    // TODO: Arguably, this is a needless iteration, we should really be iterating over only the
    // changed values
    auto maybeItems = mInFlightItems.GetBucket(clientId);
    if (!maybeItems)
    {
        return;
    }

    ConfigPairList oldItems;
    auto items = maybeItems.get();
    for (const auto& item : items)
    {
        auto old_item = UnwrapSinglePair(mSystemStore->Get(item.GetKey()));
        // We don't need to save last.* versions of defaults
        if (item.GetKey().SubkeyMatches(ConfigKey("default.*")) ||
            item.GetKey().SubkeyMatches(ConfigKey("last.*")))
        {
            continue;
        }
        ConfigKey last_key("last." + item.GetKey().ToString());
        if (old_item)
        {
            oldItems.push_back(ConfigPair(last_key, old_item.get().GetValue()));
        }
        else
        {
            oldItems.push_back(ConfigPair(last_key));
        }
    }

    for (const auto& last : oldItems)
    {
        mInFlightItems.Insert(clientId, last);
    }
}
