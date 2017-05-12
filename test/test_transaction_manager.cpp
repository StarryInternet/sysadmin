#include <log4cxx/logger.h>
#include <gtest/gtest.h>

#include "TransactionManager.h"
#include "Configurator.h"

#include "Helpers.h"

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("test_transaction_manager"));
}

class TransactionManagerFixture : public ::testing::Test
{
public:
    TransactionManagerFixture()
      : ::testing::Test()
      , mStorage()
      , mTransactionStorage()
      , mConf(&mStorage, &mTransactionStorage)
    {
    }

    virtual void SetUp()
    {
        mStorage.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("cheesywaffle")), 0);
        mStorage.Set(ConfigPair("wireless.cheese", ConfigType<bool>(true)), 0);
        mStorage.Set(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(5)), 0);
        mStorage.Commit(0);
    }

    MockSystemStorage mStorage;
    MockTransactionStorage mTransactionStorage;
    Configurator mConf;
};

class AllCommitHistoriesFixture : public ::testing::Test
{
    public:
        AllCommitHistoriesFixture()
        : ::testing::Test()
        {
        }

        CommitHistory CreateCommit(CommitHistory::CommitId commitId, CommitHistory::CommitId parentId)
        {
            CommitHistory commit;
            commit.mId = commitId;
            commit.mParent = parentId;
            return commit;
        }
};

TEST_F(TransactionManagerFixture, Basic)
{
    TransactionManager manager(&mConf, &mTransactionStorage, 3);

    ConfigPairList commit1;
    commit1.emplace_back(ConfigPair("wireless.waffle", ConfigType<std::string>("otherthing")));
    manager.LogCommit(commit1);
    ASSERT_EQ(1, manager.GetCommitCount());
    // We're not actually doing commits, so we need to manually set the value here
    mStorage.Set(ConfigPair("wireless.waffle", ConfigType<std::string>("otherthing")), 0);
    mStorage.Commit(0);

    ConfigPairList commit2;
    commit2.emplace_back(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(9)));
    manager.LogCommit(commit2);
    ASSERT_EQ(2, manager.GetCommitCount());

    // I should now be able to rollback the first commit since the second commit doesn't affect it
    ASSERT_NO_THROW(manager.Rollback(1).value());
    ASSERT_EQ("cheesywaffle", UnwrapSinglePair(mStorage.Get(ConfigPair::Key("wireless.waffle"))).get().UnpackValue<std::string>());

    ConfigPairList commit3;
    commit3.emplace_back(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(8)));
    manager.LogCommit(commit3);
    ASSERT_EQ(3, manager.GetCommitCount());

    // Rolling back commit 2 should throw an error since "wireless.dynamite.explode" was changed in commit 3
    ASSERT_THROW(manager.Rollback(2).value(), RollbackError);

    ConfigPairList commit4;
    commit4.emplace_back(ConfigPair("wireless.dynamite.explode", ConfigType<int32_t>(9)));
    manager.LogCommit(commit4);

    //GetCommitCount should still be 3 because 3 is the max number of commits
    ASSERT_EQ(3, manager.GetCommitCount());

    // After the above commit the first commit should have been dropped
    ASSERT_FALSE(mTransactionStorage.LoadCommit(1));


    //Now test blame functionality
    ASSERT_EQ(0, manager.Blame(ConfigPair::Key("key1")).size());

    ConfigPairList commit5;
    commit5.emplace_back(ConfigPair("key1", "val1"));
    commit5.emplace_back(ConfigPair("key2", "val2"));
    CommitHistory::CommitId commit5Id = manager.LogCommit(commit5);

    ASSERT_EQ(1, manager.Blame(ConfigPair::Key("key1")).size());
    ASSERT_EQ(1, manager.Blame(ConfigPair::Key("key2")).size());

    ConfigPairList commit6;
    commit6.emplace_back(ConfigPair("key1", "val5"));
    CommitHistory::CommitId commit6Id = manager.LogCommit(commit6);

    //key1 blame should list commit6 -> commit5 (youngest to oldest)
    ASSERT_EQ(2, manager.Blame(ConfigPair::Key("key1")).size());
    ASSERT_EQ(commit6Id, manager.Blame(ConfigPair::Key("key1"))[0].mCommitId);
    ASSERT_EQ(commit5Id, manager.Blame(ConfigPair::Key("key1"))[1].mCommitId);

    //key2 blame should only list commit5
    ASSERT_EQ(1, manager.Blame(ConfigPair::Key("key2")).size());
    ASSERT_EQ(commit5Id, manager.Blame(ConfigPair::Key("key2"))[0].mCommitId);
}

TEST_F(AllCommitHistoriesFixture, Basic)
{
    //Create initial commits in order (2 -> 3 -> 1) from youngest to oldest.
    std::vector<CommitHistory> initialCommits;
    initialCommits.emplace_back(CreateCommit(1, 4));
    initialCommits.emplace_back(CreateCommit(2, 3));
    initialCommits.emplace_back(CreateCommit(3, 1));

    AllCommitHistories commitHistory(initialCommits);

    //There should only be the 3 initial commits
    ASSERT_EQ(3, commitHistory.GetCommitCount());

    //Commit 2 is the most recent
    ASSERT_EQ(2, commitHistory.GetMostRecentCommit().get().mId);

    //Commit 4 is the highest commit id. Even though it isn't in use by a current commit,
    //it is listed as a parent and thus unavailable for a new commit
    ASSERT_EQ(4, commitHistory.GetHighestCommitId());

    //Commit 1 is the parent of commit 3
    ASSERT_EQ(1, commitHistory.GetCommit(3).get().mParent);

    //Droppping the oldest commit will drop commit 1. Commits will be (2 -> 3)
    ASSERT_EQ(1, commitHistory.DropOldestCommit().get().mId);

    //Ensure there's only 2 commits
    ASSERT_EQ(2, commitHistory.GetCommitCount());

    //Create and store a new commit 5. Commits will be (5 -> 2 -> 3)
    CommitHistory newCommit = CreateCommit(5, 2);
    commitHistory.StoreCommit(newCommit);

    //New commit 5 is now the most recent commit
    ASSERT_EQ(5, commitHistory.GetMostRecentCommit().get().mId);

    //Commit 5 is now the highest commit id
    ASSERT_EQ(5, commitHistory.GetHighestCommitId());

    //There's now 3 commits again
    ASSERT_EQ(3, commitHistory.GetCommitCount());
}
