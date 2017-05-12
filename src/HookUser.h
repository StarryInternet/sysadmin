
#pragma once

#include "IExternalRunner.h"
#include "ConfigTypes.h"
#include "HookConfig.h"
#include "Utilities.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_set>
#include <stdexcept>

enum HookType
{
    TEMPLATER,
    SERVICE
};

class HookUserError : public std::runtime_error {
public:
    HookUserError(const std::string& str)
      : std::runtime_error(str)
    {}
};

// HookUsers are either Templates or Services, distinguished only by what IExternalRunner they're
// given
class HookUser
{
public:
    typedef std::unordered_set<HookConfig, HookConfigHasher> HookContainer;

    typedef std::function<folly::Future<std::vector<ConfigPair::Key>> (const HookContainer&)> HookFulfiller;

public:
    HookUser(std::string name,
             HookType type,
             std::vector<HookConfig> hooks,
             HookFulfiller hookfulfiller,
             std::shared_ptr<IExternalRunner> externalScript,
             size_t run_level);

    HookUser(const HookUser& source);

    HookUser& operator=(HookUser source);

    folly::Future<folly::Unit> ExecuteExternalProcess() const;

    bool HasHook(const ConfigPair::Key& key) const;

    size_t RunLevel() const;
    std::string Name() const;
    HookType GetType() const;

    friend class HookHasher;
    friend class HookTable;

    friend bool operator==(const HookUser& lhs, const HookUser& rhs);
    friend bool operator!=(const HookUser& lhs, const HookUser& rhs);
    friend bool operator<(const HookUser& lhs, const HookUser& rhs);
    friend bool operator<=(const HookUser& lhs, const HookUser& rhs);

private:
    std::string mName;
    HookType mType;
    // All the hooks tied to this template, primarily to retrieve them from sysadmin when dumping
    HookContainer mHooks;
    HookFulfiller mHookFulfiller;
    std::shared_ptr<IExternalRunner> mExternalScript;
    size_t mRunLevel;
};

class HookHasher
{
public:
    inline size_t operator()(const HookUser& hookuser) const
    {
        size_t hash = 0;
        for (const auto& hook : hookuser.mHooks)
        {
            hash_combine(hash, hook.mKey.ToString());
        }
        hash_combine(hash, hookuser.mExternalScript);
        return hash;
    }

    inline size_t operator()(const std::shared_ptr<const HookUser>& hookuser) const
    {
        size_t hash = 0;
        for (const auto& hook : hookuser->mHooks)
        {
            hash_combine(hash, hook.mKey.ToString());
        }
        hash_combine(hash, hookuser->mExternalScript);
        return hash;
    }
};

bool operator==(const HookUser& lhs, const HookUser& rhs);
bool operator!=(const HookUser& lhs, const HookUser& rhs);
bool operator<(const HookUser& lsh, const HookUser& rhs);
bool operator<=(const HookUser& lsh, const HookUser& rhs);
