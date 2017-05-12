
#pragma once

#include <gmock/gmock.h>

#include "ICommandInterface.h"
#include "IConfigurator.h"
#include "IHistoryStorage.h"
#include "CommitHistory.h"
#include "LockedConfigPairMap.h"

#include <boost/filesystem.hpp>
#include <log4cxx/logger.h>

#include <map>
#include <list>

#define REQUIRE_UNORDERED_EQUALS(a, b) ASSERT_THAT((a), ::testing::UnorderedElementsAreArray(b));

class LocalWritingFixture : public ::testing::Test
{
public:
    LocalWritingFixture()
      : ::testing::Test(), mTmpPath(boost::filesystem::temp_directory_path() / boost::filesystem::unique_path())
    {
    }

    virtual void SetUp()
    {
        LOG4CXX_INFO(log4cxx::Logger::getLogger("LocalWritingFixture"), "Creating temp dir at: " << mTmpPath);
        boost::filesystem::create_directory(mTmpPath);
    }

    virtual void TearDown()
    {
        boost::filesystem::remove_all(mTmpPath);
    }

    boost::filesystem::path mTmpPath;
};

#define ASSERT_SET_SUCCESS(ACTION) \
    ASSERT_EQ(IConfigurator::SUCCESS, (ACTION));

#define ASSERT_SET_SUCCESS_NEW_KEY(ACTION) \
    ASSERT_EQ(IConfigurator::SUCCESS_NEW_KEY, (ACTION));

#define ASSERT_SET_TYPE_MISMATCH(ACTION) \
    ASSERT_EQ(IConfigurator::TYPE_MISMATCH, (ACTION));

#define ASSERT_SET_LOCKED(ACTION) \
    ASSERT_EQ(IConfigurator::LOCKED_QUEUE, (ACTION));

class MockSystemStorage : public IConfigurator
{
public:
    MockSystemStorage()
      : mStorage()
      , mInFlightItems(100, this)
    {
    }

    IConfigurator::SetStatus Set(const ConfigPair& item, IConfigurator::ClientId id)
    {
        mInFlightItems.Insert(id, item);
        return IConfigurator::SUCCESS;
    }

    ConfigPairMap Get(const ConfigPair::Key& key) const
    {
        ConfigPairMap output;
        if (key.IsWildcard())
        {
            auto subKeys = GetAllKeys(key);
            for (const auto& subkey : subKeys)
            {
                auto res = InternalGet(subkey);
                if (res)
                {
                    output.emplace(subkey, res.get());
                }
            }
        }
        else
        {
            auto res = InternalGet(key);
            if (res)
            {
                output.emplace(key, res.get());
            }
        }

        return output;
    }

    boost::optional<ConfigPair> InternalGet(const ConfigPair::Key& key) const
    {
        if (!mStorage.count(key))
        {
            return boost::optional<ConfigPair>();
        }
        return boost::optional<ConfigPair>(mStorage.at(key));
    }

    bool ValueExists(const ConfigPair::Key& key) const
    {
        return mStorage.count(key) >= 1;
    }

    bool ValueExists(const std::string& key) const
    {
        return ValueExists(ConfigPair::Key(key));
    }

    bool TypeCheck(const ConfigPair& pair) const
    {
        if (ValueExists(pair.GetKey()))
        {
            auto existing = UnwrapSinglePair(Get(pair.GetKey())).get();
            return existing.TypeCheckValues(pair);
        }
        return true;
    }

    void SetThrough(const ConfigPair& item)
    {
        if (mStorage.count(item.GetKey()))
        {
            mStorage.erase(item.GetKey());
        }
        if (!item.GetValue().Empty())
        {
            mStorage.emplace(item.GetKey(), item);
        }
    }

    ConfigPairMap GetAll() const
    {
        ConfigPairMap output;
        for (const auto& yv : mStorage)
        {
            output.emplace(yv.first, yv.second);
        }
        return output;
    }

    std::vector<ConfigPair::Key> GetAllKeys(const ConfigPair::Key& ns) const
    {
        std::vector<ConfigPair::Key> output;
        for (const auto& yv : mStorage)
        {
            if (yv.second.GetKey().SubkeyMatches(ns))
            {
                output.push_back(yv.second.GetKey());
            }
        }
        return output;
    }


    folly::Future<CommitHistory::CommitId>
    Commit(size_t id, sysadminctl::CommitConfig = sysadminctl::CommitConfig::DEFAULT)
    {
        auto toCommit = mInFlightItems.GetBucket(id).get();
        for (const auto& item : toCommit)
        {
            SetThrough(item);
        }
        Drop(id);
        return folly::makeFuture(CommitHistory::CommitId(0));
    }

    void Drop(size_t id)
    {
        mInFlightItems.Drop(id);
    }

    void Erase(const ConfigPair::Key& key, size_t id)
    {
        mInFlightItems.Erase(id, key);
    }

    ConfigPairList InFlightItems(size_t id)
    {
        auto maybebucket = mInFlightItems.GetBucket(id);
        if (!maybebucket)
        {
            return ConfigPairList();
        }

        return maybebucket.get();
    }

    std::map<ConfigPair::Key, ConfigPair> mStorage;
    LockedConfigPairMap mInFlightItems;
};

class MockTransactionStorage : public IHistoryStorage
{
public:
    MockTransactionStorage()
      : mCommitCount(0)
      , mCommits()
    {
    }

    size_t GetCommitCount()
    {
        return mCommitCount;
    }

    void StoreCommit(const CommitHistory& commit)
    {
        mCommits.emplace(++mCommitCount, commit);
    }

    boost::optional<CommitHistory> LoadCommit(CommitHistory::CommitId id)
    {
        if (mCommits.count(id))
        {
            return boost::optional<CommitHistory>(mCommits.at(id));
        }
        return boost::optional<CommitHistory>();
    }

    std::vector<CommitHistory> LoadAllCommits()
    {
        std::vector<CommitHistory> allCommits;
        for(auto const& ent : mCommits)
        {
            allCommits.emplace_back(ent.second);
        }
        return allCommits;
    }

    void DropCommit(CommitHistory::CommitId id)
    {
        mCommits.erase(id);
    }

    size_t mCommitCount;
    std::map<CommitHistory::CommitId, CommitHistory> mCommits;
};
