
#pragma once

#include <unordered_map>
#include <list>

template <typename Key, typename Value>
class SizeAgeBoundMap
{
public:
    SizeAgeBoundMap(size_t max)
      : mItems()
      , mHistory()
      , mLookups()
      , mMaxSize(max)
    {}

    bool Insert(const Key& key, const Value& value)
    {
        if (!mItems.count(key))
        {
            if (mItems.size() >= mMaxSize)
            {
                AgeOut();
            }
            mItems.emplace(key, value);
            mHistory.push_back(key);
            mLookups.emplace(key, --mHistory.end());
            return true;
        }
        return false;
    }

    Value& At(const Key& key)
    {
        return mItems.at(key);
    }

    void Erase(const Key& key)
    {
        mItems.erase(key);
        mHistory.erase(mLookups.at(key));
        mLookups.erase(key);
    }

    size_t Count(const Key& key) const
    {
        return mItems.count(key);
    }

    size_t Size() const
    {
        return mItems.size();
    }

private:
    void AgeOut()
    {
        Key& oldest = mHistory.front();
        mItems.erase(oldest);
        mLookups.erase(oldest);
        mHistory.pop_front();
    }

private:
    typedef std::unordered_map<Key, Value> Container;
    typedef std::list<Key> AgeQueue;
    typedef std::unordered_map<Key, typename AgeQueue::iterator> LookupMap;

private:
    Container mItems;
    AgeQueue mHistory;
    LookupMap mLookups;
    size_t mMaxSize;
};
