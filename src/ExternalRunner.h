
#pragma once

#include "IExternalRunner.h"
#include "HookConfig.h"

#include "decibel/messaging/Reactor.h"
#include "decibel/niceuv/FutureSubprocess.h"
#include <boost/filesystem.hpp>
#include <stdexcept>

class ExternalRunnerError : public std::runtime_error {
public:
    ExternalRunnerError(const std::string& str)
      : std::runtime_error(str)
    {}
};

class ExternalRunner : public IExternalRunner
{
public:
    ExternalRunner(decibel::messaging::Reactor* reactor,
                   const std::vector<std::string>& leadingArgs,
                   HookOptions::RunOptions options);

    virtual folly::Future<folly::Unit> Run(const IExternalRunner::FormattedValues& values) const;

private:
    mutable decibel::messaging::Reactor* mReactor;
    std::vector<std::string> mLeadingArgs;
    mutable std::shared_ptr<decibel::niceuv::FutureSubprocess> mSubproc;
    HookOptions::RunOptions mRunOptions;
};

decibel::niceuv::SubprocessHandler::SubprocessOptions ConvertOptions(HookOptions::RunOptions options);
