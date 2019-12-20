
#include "ExternalRunner.h"

#include <folly/futures/Promise.h>

#include <log4cxx/logger.h>

#include <sstream>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("ExternalRunner"));
}

namespace bfs = boost::filesystem;
namespace dm = decibel::messaging;
namespace dn = decibel::niceuv;

ExternalRunner::ExternalRunner(dm::Reactor* reactor, const std::vector<std::string>& leadingArgs,
                               HookOptions::RunOptions options)
  : mReactor(reactor)
  , mLeadingArgs(leadingArgs)
  , mSubproc()
  , mRunOptions(options)
{
}

folly::Future<folly::Unit> ExternalRunner::Run(const IExternalRunner::FormattedValues& values) const
{
    std::stringstream basic_process;
    for (const auto& c : mLeadingArgs)
    {
        basic_process << c << " ";
    }
    const std::string basic_process_str = basic_process.str();
    LOG4CXX_INFO(spLogger, "Running external process: " << basic_process_str);

    HookOptions::RunOptions options_copy = mRunOptions;
    std::vector<std::string> cmd(mLeadingArgs.begin(), mLeadingArgs.end());
    cmd.insert(cmd.end(), values.begin(), values.end());
    mSubproc.reset(new dn::FutureSubprocess(mReactor->GetEventLoop(),
                                            cmd,
                                            ConvertOptions(mRunOptions)));
    return mSubproc->RunSubprocess()
        .thenValue([basic_process_str](auto /*unused*/)
        {
            LOG4CXX_INFO(spLogger, "External process completed: " << basic_process_str);
        }).thenError(folly::tag_t<dn::FutureSubprocessError>{}, [options_copy, cmd](const auto& err)
        {
            if (options_copy == HookOptions::RunOptions::IGNORE)
            {
                return;
            }
            std::stringstream cmdrun;
            for (const auto& c : cmd)
            {
                cmdrun << c << " ";
            }
            LOG4CXX_ERROR(spLogger, "External Process failed at runtime: " << cmdrun.str());
            std::stringstream stdout;
            if (err.mBuffer.size() > 0)
            {
                stdout << "\n";
            }
            for (const auto& c : err.mBuffer)
            {
                stdout << c;
            }
            LOG4CXX_ERROR(spLogger, "stdout from err'd process: " << stdout.str());
            std::stringstream stderr;
            if (err.mErrBuffer.size() > 0)
            {
                stderr << "\n";
            }
            for (const auto& c : err.mErrBuffer)
            {
                stderr << c;
            }
            LOG4CXX_ERROR(spLogger, "stderr from err'd process: " << stderr.str());
            throw ExternalRunnerError(err.what());
        });
}

dn::SubprocessHandler::SubprocessOptions ConvertOptions(HookOptions::RunOptions options)
{
    dn::SubprocessHandler::SubprocessOptions out_options = {};
    if (options == HookOptions::RunOptions::DETACH)
    {
        out_options.push_back(
            dn::SubprocessHandler::SubprocessOption::UV_PROCESS_DETACHED
        );
        out_options.push_back(
            dn::SubprocessHandler::SubprocessOption::STARRY_PROCESS_FULLY_DETACH
        );
    }
    return out_options;
}
