
#pragma once

#include "IHistoryStorage.h"
#include <boost/filesystem.hpp>

boost::filesystem::path TranslateIdToPath(boost::filesystem::path root,
                                          CommitHistory::CommitId id);

class LocalHistoryStorage : public IHistoryStorage
{
public:
    LocalHistoryStorage(const std::string& storageDir, CommitSerializer serializer,
                        CommitDeserializer deserializer);

    size_t GetCommitCount();

    void StoreCommit(const CommitHistory& commit);

    boost::optional<CommitHistory> LoadCommit(CommitHistory::CommitId id);

    std::vector<CommitHistory> LoadAllCommits();

    void DropCommit(CommitHistory::CommitId id);

private:
    boost::optional<CommitHistory> LoadCommit(boost::filesystem::path path);

private:
    boost::filesystem::path mStorageDir;
    CommitSerializer mSerializer;
    CommitDeserializer mDeserializer;
};
