
#include "LocalHistoryStorage.h"

#include <algorithm>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <folly/File.h>
#include <folly/FileUtil.h>

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <log4cxx/logger.h>

namespace bfs = boost::filesystem;

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("LocalHistoryStorage"));
}

bfs::path TranslateIdToPath(bfs::path root, CommitHistory::CommitId id)
{
    bfs::path commitpath = root / bfs::path(boost::lexical_cast<std::string>(id));
    commitpath.replace_extension(".commit");
    return commitpath;
}

bool compare_commit_files(const bfs::path& p1, const bfs::path& p2)
{
    size_t ps1 = boost::lexical_cast<size_t>(p1.stem().string());
    size_t ps2 = boost::lexical_cast<size_t>(p2.stem().string());
    return ps1 < ps2;
}

LocalHistoryStorage::LocalHistoryStorage(const std::string& storageDir,
                                         CommitSerializer serializer,
                                         CommitDeserializer deserializer)
  : mStorageDir(storageDir)
  , mSerializer(serializer)
  , mDeserializer(deserializer)
{
    if (!bfs::is_directory(mStorageDir))
    {
        bfs::create_directories(mStorageDir);
    }
}

size_t LocalHistoryStorage::GetCommitCount()
{
    std::vector<bfs::path> paths;
    std::copy(bfs::directory_iterator(mStorageDir), bfs::directory_iterator(),
              std::back_inserter(paths));
    std::sort(paths.begin(), paths.end(), compare_commit_files);
    if (paths.size() > 0)
    {
        bfs::path latest = paths.back();
        latest.replace_extension("");

        return boost::lexical_cast<size_t>(latest.stem().string());
    }
    return 0;
}

void LocalHistoryStorage::StoreCommit(const CommitHistory& commit)
{
    boost::optional<std::string> maybeBuffer = mSerializer(commit);
    if (!maybeBuffer)
    {
        LOG4CXX_ERROR(spLogger, "Couldn't serialize commit");
        return;
    }

    bfs::path commitpath = TranslateIdToPath(mStorageDir, commit.mId);
    if (bfs::exists(commitpath))
    {
        LOG4CXX_WARN(spLogger, "Writing over existing commit file: " << commitpath.string());
    }

    auto fout = folly::File(commitpath.string(), O_WRONLY | O_CREAT | O_TRUNC);
    if (fout)
    {
        folly::writeFull(fout.fd(), maybeBuffer.get().c_str(), maybeBuffer.get().size());
        folly::fsyncNoInt(fout.fd());
    }
    else
    {
        LOG4CXX_ERROR(spLogger, "Couldn't open file for writing: " << commitpath.string());
    }
    if (!fout.closeNoThrow())
    {
        LOG4CXX_ERROR(spLogger, "Couldn't close file: " << commitpath.string());
    }
}

boost::optional<CommitHistory> LocalHistoryStorage::LoadCommit(CommitHistory::CommitId id)
{
    bfs::path commitpath = TranslateIdToPath(mStorageDir, id);
    return LoadCommit(commitpath);
}

std::vector<CommitHistory> LocalHistoryStorage::LoadAllCommits()
{
    std::vector<CommitHistory> allCommits;

    std::vector<bfs::path> paths;
    std::copy(bfs::directory_iterator(mStorageDir), bfs::directory_iterator(),
              std::back_inserter(paths));
    std::sort(paths.begin(), paths.end(), compare_commit_files);
    for (bfs::path path : paths)
    {
        boost::optional<CommitHistory> maybeCommit = LoadCommit(path);
        if (maybeCommit)
        {
            allCommits.push_back(maybeCommit.get());
        }
    }
    return allCommits;
}

boost::optional<CommitHistory> LocalHistoryStorage::LoadCommit(boost::filesystem::path commitpath) {
    if (!bfs::exists(commitpath))
    {
        LOG4CXX_WARN(spLogger, "Can't find commit at path " << commitpath.string());
        return boost::optional<CommitHistory>();
    }

    std::ifstream ifs(commitpath.string());
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));

    boost::optional<CommitHistory> maybeCommit = mDeserializer(content);
    if (!maybeCommit)
    {
        LOG4CXX_ERROR(spLogger, "Couldn't deserialize commit");
    }
    return maybeCommit;
}

void LocalHistoryStorage::DropCommit(CommitHistory::CommitId id)
{
    bfs::path commitpath = TranslateIdToPath(mStorageDir, id);

    bfs::remove(commitpath);
}
