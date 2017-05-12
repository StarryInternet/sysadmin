
#include "LockedConfigPairMap.h"

LockedConfigPairMap::LockedConfigPairMap(size_t max_size, IConfigurator* pDb)
  : mStorage(max_size)
  , mpDb(pDb)
{
}

void LockedConfigPairMap::Insert(BucketId bucket, const ConfigPair& newItem)
{
    if (!mStorage.Count(bucket))
    {
        mStorage.Insert(bucket, LockedConfigPairList());
    }
    mStorage.At(bucket).mList.push_back(newItem);
}

void LockedConfigPairMap::Erase(BucketId bucket, const ConfigPair::Key& key)
{
    if (key.IsWildcard())
    {
        auto subkeys = mpDb->GetAllKeys(key);
        for (const auto& key : subkeys)
        {
            // Don't allow wildcard deletion of last.* or default.* keys
            if (key.SubkeyMatches(ConfigKey("last.*")) ||
                key.SubkeyMatches(ConfigKey("default.*")))
            {
                continue;
            }
            Insert(bucket, ConfigPair(key));
        }
    }
    else
    {
        Insert(bucket, ConfigPair(key));
    }
}

void LockedConfigPairMap::Drop(BucketId bucket)
{
    if (mStorage.Count(bucket))
    {
        mStorage.Erase(bucket);
    }
}

void LockedConfigPairMap::LockBucket(BucketId bucket)
{
    if (mStorage.Count(bucket))
    {
        mStorage.At(bucket).mLocked = true;
    }
}

void LockedConfigPairMap::UnlockBucket(BucketId bucket)
{
    if (mStorage.Count(bucket))
    {
        mStorage.At(bucket).mLocked = false;
    }
}

bool LockedConfigPairMap::IsBucketLocked(BucketId bucket)
{
    if (mStorage.Count(bucket))
    {
        return mStorage.At(bucket).mLocked;
    }
    return false;
}

bool LockedConfigPairMap::Exists(BucketId bucket)
{
    return bool(mStorage.Count(bucket));
}

boost::optional<ConfigPairList> LockedConfigPairMap::GetBucket(BucketId bucket)
{
    if (mStorage.Count(bucket))
    {
        auto list = mStorage.At(bucket).mList;
        return boost::make_optional(ConfigPairList(list.begin(), list.end()));
    }

    return boost::none;
}
