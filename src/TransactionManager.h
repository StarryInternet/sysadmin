
#pragma once

#include "CommitHistory.h"
#include "ConfigTypes.h"
#include "ICommandInterface.h"
#include "IConfigurator.h"
#include "IHistoryStorage.h"
#include "decibel/messaging/Protocol.h"

class RollbackError : public std::runtime_error
{
public:
    RollbackError(const std::string& str)
      : std::runtime_error(str)
    {}
};

//Helper class for sorting CommitHistorys on init
class AllCommitHistories
{
public:
    AllCommitHistories(const std::vector<CommitHistory>& commits);

    void StoreCommit(const CommitHistory& commit);
    
    boost::optional<CommitHistory> GetCommit(CommitHistory::CommitId commitId);
    boost::optional<CommitHistory> GetChild(CommitHistory::CommitId parentId);
    boost::optional<CommitHistory> DropOldestCommit();
    boost::optional<CommitHistory> GetMostRecentCommit();
    CommitHistory::CommitId GetHighestCommitId();
    size_t GetCommitCount();
    std::vector<BlameDelta> Blame(const ConfigPair::Key& key);
private:
    CommitHistory::CommitId FindMaxCommitId(const std::vector<CommitHistory>& unsortedCommits);
    std::list<CommitHistory> Sort(const std::vector<CommitHistory>& unsortedCommits);
    boost::optional<CommitHistory> FindMostRecentCommit(const std::vector<CommitHistory>& unsortedCommits);
    boost::optional<CommitHistory> FindCommit(CommitHistory::CommitId commitId, const std::vector<CommitHistory>& unsortedCommits);
    boost::optional<CommitHistory> FindChild(CommitHistory::CommitId parentId, const std::vector<CommitHistory>& unsortedCommits);
private:
    std::list<CommitHistory> mCommits;
    CommitHistory::CommitId mMaxCommitId;
};

class TransactionManager
{
public:
    TransactionManager(IConfigurator* pDb,
                       IHistoryStorage* pStorage, size_t maxStorage);

    CommitHistory::CommitId LogCommit(const ConfigPairList& toBeCommitted);

    folly::Future<folly::Unit> Rollback(CommitHistory::CommitId commitId);

    size_t GetCommitCount() { return mCommitHistory.GetCommitCount(); }

    std::vector<BlameDelta> Blame(const ConfigPair::Key& key) { return mCommitHistory.Blame(key); }

private:
    CommitHistory::ConfigDeltaList GenerateDeltas(const ConfigPairList& old);

    void DropExcessiveCommits();
    bool CanRollback(const CommitHistory& commitId);

public:
    static const decibel::messaging::Protocol::Id TRANSACTION_HISTORY_ID = 5;

private:
    IConfigurator* mpDb;
    IHistoryStorage* mStorage;
    size_t mMaxStorage;
    AllCommitHistories mCommitHistory;
};
