#pragma once

#include <boost/optional.hpp>

#include "IConfigurator.h"
#include "ConfigTypes.h"
#include "SizeAgeBoundMap.h"

struct LockedConfigPairList
{
    LockedConfigPairList()
      : mLocked(false)
      , mList()
    {
    };

    bool mLocked;
    ConfigPairList mList;
};

// TODO: I've tried this before and been too lazy to finish it, but if I use a map of
// unordered maps here instead of a map of lists, I don't have to simplify the queue
// as I do in PruneUnchangedValues now
class LockedConfigPairMap
{
public:
    typedef size_t BucketId;

public:
    LockedConfigPairMap(size_t max_size, IConfigurator* pDb);

    void Insert(BucketId bucket, const ConfigPair& newItem);

    void Erase(BucketId bucket, const ConfigPair::Key& key);

    void Drop(BucketId bucket);

    void LockBucket(BucketId bucket);

    void UnlockBucket(BucketId bucket);

    bool IsBucketLocked(BucketId bucket);

    bool Exists(BucketId bucket);

    boost::optional<ConfigPairList> GetBucket(BucketId bucket);

private:
    SizeAgeBoundMap<BucketId, LockedConfigPairList> mStorage;
    // I _think_, once there are only IConfigurators, I will create a simpler
    // abstract pass through Configurator pipeline unit that will handle
    // wildcards, which will then make this pointer unnecessary. For now,
    // the goal is to have every step of this change actually function
    IConfigurator* mpDb;
};
