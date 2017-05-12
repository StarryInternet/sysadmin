
#include "HookConfig.h"
#include "HookUser.h"
#include "KeyValueFormatter.h"
#include "ExternalRunner.h"

#include <algorithm>
#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("HookUser"));
}

HookUser::HookUser(std::string name,
                   HookType type,
                   std::vector<HookConfig> hooks,
                   HookFulfiller hookfulfiller,
                   std::shared_ptr<IExternalRunner> externalScript,
                   size_t run_level)
  : mName(name)
  , mType(type)
  , mHooks(hooks.begin(), hooks.end())
  , mHookFulfiller(hookfulfiller)
  , mExternalScript(externalScript)
  , mRunLevel(run_level)
{
}

HookUser::HookUser(const HookUser& source)
  : mName(source.mName)
  , mType(source.mType)
  , mHooks(source.mHooks.begin(), source.mHooks.end())
  , mHookFulfiller(source.mHookFulfiller)
  , mExternalScript(source.mExternalScript)
  , mRunLevel(source.mRunLevel)
{
}

HookUser& HookUser::operator=(HookUser source)
{
    if (this != &source)
    {
        mName = source.mName;
        mType = source.mType;
        mHooks.swap(source.mHooks);
        mHookFulfiller.swap(source.mHookFulfiller);
        mExternalScript = source.mExternalScript;
        mRunLevel = source.mRunLevel;
    }

    return *this;
}

folly::Future<folly::Unit> HookUser::ExecuteExternalProcess() const
{
    return mHookFulfiller(mHooks).then([this](const std::vector<ConfigPair::Key>& )
    {
        // We don't pass anything to hooks being run
        return this->mExternalScript->Run({});
    }).onError([](const HookUserError& err)
    {
        throw ExternalRunnerError(err.what());
    });
}

bool HookUser::HasHook(const ConfigPair::Key& key) const
{
    for (const auto& hook : mHooks)
    {
        if (key == hook.mKey || (hook.mKey.IsWildcard() && key.SubkeyMatches(hook.mKey)))
        {
            return true;
        }
    }
    return false;
}

size_t HookUser::RunLevel() const
{
    return mRunLevel;
}

std::string HookUser::Name() const
{
    return mName;
}

HookType HookUser::GetType() const
{
  return mType;
}

bool operator==(const HookUser& lhs, const HookUser& rhs)
{
    return lhs.mHooks == rhs.mHooks && lhs.mExternalScript == rhs.mExternalScript;
}

bool operator!=(const HookUser& lhs, const HookUser& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const HookUser& lhs, const HookUser& rhs)
{
    return lhs.mRunLevel < rhs.mRunLevel;
}

bool operator<=(const HookUser& lhs, const HookUser& rhs)
{
    return lhs.mRunLevel < rhs.mRunLevel;
}
