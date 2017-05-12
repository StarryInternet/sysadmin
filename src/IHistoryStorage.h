
#pragma once

#include "CommitHistory.h"

class IHistoryStorage
{
public:
    virtual size_t GetCommitCount() = 0;

    virtual void StoreCommit(const CommitHistory& commit) = 0;

    virtual boost::optional<CommitHistory> LoadCommit(CommitHistory::CommitId id) = 0;

    virtual std::vector<CommitHistory> LoadAllCommits() = 0;

    virtual void DropCommit(CommitHistory::CommitId id) = 0;
};
