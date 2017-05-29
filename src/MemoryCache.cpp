
#include "MemoryCache.h"

#include <boost/assert.hpp>

namespace dm = decibel::messaging;

MemoryCache::MemoryCache(IConfigurator* backingStore)
  : mBackingStore(backingStore)
  , mValues()
  , mInFlightItems(100)
{
    auto allValues = mBackingStore->GetAll();
    for (const auto& value : allValues)
    {
        mValues.emplace(value.first, DirtyConfigPair(value.second));
    }
}

IConfigurator::SetStatus MemoryCache::Set(const ConfigPair& value, dm::Protocol::Id id)
{
    mInFlightItems.Insert(id, value);
    return IConfigurator::SUCCESS;
}

ConfigPairMap MemoryCache::Get(const ConfigPair::Key& key) const
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

boost::optional<ConfigPair> MemoryCache::InternalGet(const ConfigPair::Key& key) const
{
    if (mValues.count(key))
    {
        return boost::make_optional(mValues.at(key).mData);
    }
    return boost::none;
}

bool MemoryCache::TypeCheck(const ConfigPair& comp) const
{
    auto existing = UnwrapSinglePair(Get(comp.GetKey()));
    if (!existing)
    {
        return true;
    }
    return existing.get().TypeCheckValues(comp);
}

ConfigPairMap MemoryCache::GetAll() const
{
    ConfigPairMap output;
    for (const auto& value : mValues)
    {
        output.emplace(value.first, value.second.mData);
    }

    return output;
}

std::vector<ConfigPair::Key> MemoryCache::GetAllKeys(const ConfigPair::Key& ns) const
{
    BOOST_ASSERT(ns.IsWildcard());
    std::vector<ConfigPair::Key> output;
    for (const auto& value : mValues)
    {
        if (value.second.mData.GetKey().SubkeyMatches(ns))
        {
            output.push_back(value.second.mData.GetKey());
        }
    }
    return output;
}

void MemoryCache::UpdateItems(const ConfigPairList& items)
{
    for (const auto& item : items)
    {
        if (mValues.count(item.GetKey()))
        {
            mValues.erase(item.GetKey());
        }
        if (!item.GetValue().Empty())
        {
            mValues.emplace(item.GetKey(), DirtyConfigPair(item));
        }
    }
}

folly::Future<CommitHistory::CommitId>
MemoryCache::Commit(size_t id, sysadminctl::CommitConfig)
{
    auto toCommit = mInFlightItems.GetBucket(id).get();
    UpdateItems(toCommit);
    for (const auto& item : toCommit)
    {
        mBackingStore->Set(item, id);
    }
    Drop(id);
    return mBackingStore->Commit(id);
}

void MemoryCache::Drop(size_t id)
{
    mInFlightItems.Drop(id);
}

void MemoryCache::Erase(const ConfigPair::Key& key, size_t id)
{
    mInFlightItems.Erase(id, key);
}

ConfigPairList MemoryCache::InFlightItems(size_t id)
{
    auto maybebucket = mInFlightItems.GetBucket(id);
    if (!maybebucket)
    {
        return ConfigPairList();
    }

    return maybebucket.get();
}
