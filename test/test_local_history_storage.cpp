#include <log4cxx/logger.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstdio>

#include "LocalHistoryStorage.h"
#include "Helpers.h"

namespace bfs = boost::filesystem;

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_local_history_storage"));
}

void touch_file(bfs::path path)
{
    FILE* fd = fopen(path.string().c_str(), "w");
    fclose(fd);
}

TEST_F(LocalWritingFixture, CommitStoring)
{
    LocalHistoryStorage storage(mTmpPath.string(), [](const CommitHistory&)
    {
        return std::string();
    }, [](const std::string&)
    {
        return CommitHistory();
    });

    CommitHistory commit1;
    commit1.mId = 1;
    storage.StoreCommit(commit1);
    ASSERT_TRUE((bool)storage.LoadCommit(1));
    storage.DropCommit(1);
    ASSERT_FALSE((bool)storage.LoadCommit(1));

    // clearly doesn't exist
    ASSERT_FALSE((bool)storage.LoadCommit(676));
}

TEST_F(LocalWritingFixture, ExistingCommits)
{
    touch_file(mTmpPath / "5.commit");
    touch_file(mTmpPath / "9.commit");
    touch_file(mTmpPath / "10.commit");
    touch_file(mTmpPath / "12.commit");
    touch_file(mTmpPath / "100.commit");

    LocalHistoryStorage storage(mTmpPath.string(), [](const CommitHistory&)
    {
        return std::string();
    }, [](const std::string&)
    {
        return CommitHistory();
    });

    ASSERT_EQ(100, storage.GetCommitCount());
}
