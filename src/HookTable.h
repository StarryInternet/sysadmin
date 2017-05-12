#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/optional.hpp>
#include <memory>
#include <vector>

#include "HookUser.h"

class HookTable
{
private:
    // For multi_index tags
    struct Name{};
    struct Type{};
    struct RunLevel{};

public:
    typedef boost::multi_index_container<
    std::shared_ptr<HookUser>,
    boost::multi_index::indexed_by<
        boost::multi_index::hashed_unique<
            boost::multi_index::tag<Name>,
            BOOST_MULTI_INDEX_MEMBER(HookUser, std::string, mName)
            >,
        boost::multi_index::hashed_non_unique<
            boost::multi_index::tag<Type>,
            BOOST_MULTI_INDEX_MEMBER(HookUser, HookType, mType)
            >,
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<RunLevel>,
            boost::multi_index::composite_key<
                std::shared_ptr<HookUser>,
                BOOST_MULTI_INDEX_MEMBER(HookUser, HookType, mType),
                BOOST_MULTI_INDEX_MEMBER(HookUser, size_t, mRunLevel)
                >
            >
        >
    > MultiHookTable;

    typedef std::map<size_t, std::vector<std::shared_ptr<const HookUser>>> RunLevelServicesMap;
    typedef std::unordered_set<std::shared_ptr<const HookUser>, HookHasher> TemplatesSet;

public:
    HookTable();

    void Insert(HookUser&& hook);

    /*
     * I made these return types shared_ptrs to const HookUsers so we don't have
     * to create copies of HookUsers being returned, and so the entries cannot
     * be modified which would break multi_index_container's indexing of these
     * HookUsers.
     */
    boost::optional<std::shared_ptr<const HookUser>>
    ByName(const std::string& name);

    std::vector<std::shared_ptr<const HookUser>>
    ByType(const HookType type);

    std::vector<std::shared_ptr<const HookUser>>
    RunLevelOrder(const HookType type = HookType::SERVICE);

    HookTable::RunLevelServicesMap GetOrderedServices();

private:
    MultiHookTable mTable;
};
