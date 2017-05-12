
#pragma once

#include "ConfigTypes.h"

#include <functional>
#include <list>

#include <folly/futures/Future.h>

struct BlameDelta
{
    CommitHistory::CommitId mCommitId;
    boost::posix_time::ptime mCommitTime;
    CommitHistory::ConfigDelta mDelta;
};

class ICommandInterface
{
public:
    typedef std::function<folly::Future<folly::Unit> (const ConfigPairList&, bool)> CommitHook;

public:
    virtual ~ICommandInterface()
    {
    }

    virtual void RegisterCommitHook(CommitHook hook) = 0;

    virtual folly::Future<folly::Unit> Rollback(CommitHistory::CommitId id) = 0;

    virtual std::vector<BlameDelta> Blame(const ConfigPair::Key& key) = 0;
};
