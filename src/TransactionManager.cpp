
#include "TransactionManager.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <log4cxx/logger.h>

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("TransactionManager"));
}

AllCommitHistories::AllCommitHistories(const std::vector<CommitHistory>& commits)
  : mCommits(Sort(commits))
  , mMaxCommitId(FindMaxCommitId(commits))
{
}

void AllCommitHistories::StoreCommit(const CommitHistory& commit)
{
    mCommits.emplace_back(commit);
    mMaxCommitId = std::max(mMaxCommitId, commit.mId);
    mMaxCommitId = std::max(mMaxCommitId, commit.mParent);
}

boost::optional<CommitHistory> AllCommitHistories::GetCommit(CommitHistory::CommitId commitId)
{
    boost::optional<CommitHistory> maybeCommit;
    for (const auto& commit : mCommits)
    {
        if (commit.mId == commitId)
        {
            maybeCommit = commit;
        }
    }
    return maybeCommit;
}

boost::optional<CommitHistory> AllCommitHistories::GetChild(CommitHistory::CommitId parentId)
{
    boost::optional<CommitHistory> maybeCommit;
    for (const auto& commit : mCommits)
    {
        if (commit.mParent == parentId)
        {
            maybeCommit = commit;
        }
    }
    return maybeCommit;
}

boost::optional<CommitHistory> AllCommitHistories::DropOldestCommit()
{
    boost::optional<CommitHistory> maybeCommit;
    if (mCommits.size() > 0)
    {
        maybeCommit = mCommits.front();
        mCommits.erase(mCommits.begin());
    }
    return maybeCommit;
}

boost::optional<CommitHistory> AllCommitHistories::GetMostRecentCommit()
{
    boost::optional<CommitHistory> maybeCommit;
    if (mCommits.size() > 0)
    {
        maybeCommit = mCommits.back();
    }
    return maybeCommit;
}

//Returns the highest commit id in use. This is not necessarily the commit id of the most recent commit
CommitHistory::CommitId AllCommitHistories::GetHighestCommitId()
{
    return mMaxCommitId;
}

size_t AllCommitHistories::GetCommitCount()
{
    return mCommits.size();
}

std::vector<BlameDelta> AllCommitHistories::Blame(const ConfigPair::Key& key)
{
    auto getDelta = [key] (const CommitHistory& commit)
    {
        boost::optional<CommitHistory::ConfigDelta> maybeDelta;
        for (const auto& delta : commit.mItems)
        {
            if (delta.first.GetKey() == key)
            {
                maybeDelta = delta;
            }
        }
        return maybeDelta;
    };

    std::vector<BlameDelta> vec;
    for (const auto& commit : mCommits)
    {
        auto configDelta = getDelta(commit);
        if (configDelta)
        {
            BlameDelta blameDelta {commit.mId, commit.mTime, configDelta.get()};
            vec.emplace_back(blameDelta);
        }
    }
    //Reverse the vec so deltas are listed youngest -> oldest
    std::reverse(vec.begin(), vec.end());
    return vec;
}

CommitHistory::CommitId
AllCommitHistories::FindMaxCommitId(const std::vector<CommitHistory>& unsortedCommits)
{
    CommitHistory::CommitId id = 0;
    for (auto& commit : unsortedCommits)
    {
        id = std::max(id, commit.mId);
        id = std::max(id, commit.mParent);
    }
    return id;
}

boost::optional<CommitHistory>
AllCommitHistories::FindMostRecentCommit(const std::vector<CommitHistory>& unsortedCommits)
{
    boost::optional<CommitHistory> maybeCommit;
    for (const auto& commit : unsortedCommits)
    {
        auto maybeChild = FindChild(commit.mId, unsortedCommits);
        if (!maybeChild)         //The youngest commit won't have a child
        {
            maybeCommit = commit;
        }
    }
    return maybeCommit;
}

boost::optional<CommitHistory>
AllCommitHistories::FindChild(CommitHistory::CommitId parentId,
                              const std::vector<CommitHistory>& unsortedCommits)
{
    boost::optional<CommitHistory> maybeCommit;
    for (const auto& commit : unsortedCommits)
    {
        if (commit.mParent == parentId)
        {
            maybeCommit = commit;
        }
    }
    return maybeCommit;
}

std::list<CommitHistory>
AllCommitHistories::Sort(const std::vector<CommitHistory>& unsortedCommits)
{
    std::list<CommitHistory> sortedCommits;

    auto maybeCurrent = FindMostRecentCommit(unsortedCommits);
    while (maybeCurrent)
    {
        CommitHistory commit = maybeCurrent.get();
        sortedCommits.insert(sortedCommits.begin(), commit);
        maybeCurrent = FindCommit(commit.mParent, unsortedCommits);
    }

    return sortedCommits;
}

boost::optional<CommitHistory>
AllCommitHistories::FindCommit(CommitHistory::CommitId commitId,
                               const std::vector<CommitHistory>& unsortedCommits)
{
    boost::optional<CommitHistory> maybeCommit;
    for (const auto& commit : unsortedCommits)
    {
        if (commit.mId == commitId)
        {
            maybeCommit = commit;
        }
    }
    return maybeCommit;
}

TransactionManager::TransactionManager(IConfigurator* pDb,
                                       IHistoryStorage* pStorage,
                                       size_t maxStorage)
  : mpDb(pDb)
  , mStorage(pStorage)
  , mMaxStorage(maxStorage)
  , mCommitHistory(mStorage->LoadAllCommits())
{
    DropExcessiveCommits();
}

//TODO: In the future AllCommitHistory could be responsible for the local storage as well
size_t TransactionManager::LogCommit(const ConfigPairList& toBeCommitted)
{
    CommitHistory commit;
    CommitHistory::ConfigDeltaList items = GenerateDeltas(toBeCommitted);
    commit.mItems.insert(commit.mItems.begin(), items.begin(), items.end());
    commit.mId = mCommitHistory.GetHighestCommitId() + 1;

    //Assign parent id (defaults to 0 if this is the first commit)
    CommitHistory::CommitId parentId = 0;
    boost::optional<CommitHistory> parent = mCommitHistory.GetMostRecentCommit();
    if (parent)
    {
        parentId = parent.get().mId;
    }
    commit.mParent = parentId;
    commit.mTime = boost::posix_time::second_clock::local_time();

    mCommitHistory.StoreCommit(commit);
    mStorage->StoreCommit(commit);

    DropExcessiveCommits();

    return commit.mId;
}

folly::Future<folly::Unit>
TransactionManager::Rollback(CommitHistory::CommitId commitId)
{
    auto maybeCommit = mCommitHistory.GetCommit(commitId);

    if (maybeCommit)
    {
        auto commit = maybeCommit.get();
        if (CanRollback(commit))
        {
            for (const auto& item : commit.mItems)
            {
                if (item.first.GetValue().Empty())
                {
                    mpDb->Erase(item.first.GetKey(), TRANSACTION_HISTORY_ID);
                }
                else
                {
                    mpDb->Set(item.first, TRANSACTION_HISTORY_ID);
                }
            }

            return mpDb->Commit(TRANSACTION_HISTORY_ID).then([commitId](CommitHistory::CommitId)
            {
                LOG4CXX_INFO(spLogger, "Completed rollback of commit " << commitId);
                return folly::makeFuture();
            });
        }
        else
        {
            return folly::makeFuture<folly::Unit>(RollbackError("Item changed since commit"));
        }

    }

    LOG4CXX_INFO(spLogger, "Commit id " << commitId << " does not exist, can't roll it back");
    return folly::makeFuture<folly::Unit>(RollbackError("Commit id doesn't exist, can't rollback"));
}

//Returns false if any of the values written in the specified commit have been written in a more recent commit
bool TransactionManager::CanRollback(const CommitHistory& commitRolledBack)
{
    auto current = mCommitHistory.GetChild(commitRolledBack.mId);

    while (current)
    {
        CommitHistory childCommit = current.get();
        for (const auto& delta : commitRolledBack.mItems)
        {
            ConfigKey key = delta.first.GetKey();
            for (const auto& laterDelta : childCommit.mItems)
            {
                ConfigKey laterKey = laterDelta.first.GetKey();
                if (key == laterKey)
                {
                    LOG4CXX_WARN(spLogger, "Item changed since commit, can't rollback. Key: " <<
                                 key.ToString() << " changed in commit: " <<
                                 childCommit.mId);
                    return false;
                }
            }
        }
        current = mCommitHistory.GetChild(childCommit.mId);
    }

    return true;
}

//Generate a ConfigDeltaList from a ConfigPairList to create a new CommitHistory object
CommitHistory::ConfigDeltaList
TransactionManager::GenerateDeltas(const ConfigPairList& old)
{
    CommitHistory::ConfigDeltaList output;

    for (const auto& item : old)
    {
        auto maybeOld = UnwrapSinglePair(mpDb->Get(item.GetKey()));
        auto old = (maybeOld) ? maybeOld.get() : ConfigPair(item.GetKey());
        output.emplace_back(old, item);
    }

    return output;
}

//Drop oldest commits until no longer over max capacity
void TransactionManager::DropExcessiveCommits()
{
    while (mCommitHistory.GetCommitCount() > mMaxStorage)
    {
        auto maybeCommit = mCommitHistory.DropOldestCommit();
        if (maybeCommit)
        {
            mStorage->DropCommit(maybeCommit.get().mId);
        }
    }
}
