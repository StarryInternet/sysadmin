
#include "TemplaterRunner.h"

#include <folly/futures/Promise.h>

#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("TemplaterRunner"));
}

namespace bfs = boost::filesystem;
namespace dm = decibel::messaging;

TemplaterRunner::TemplaterRunner(dm::Reactor* reactor,
                                 const bfs::path& scriptPath,
                                 const bfs::path& templatePath,
                                 const bfs::path& renderedPath,
                                 const std::vector<std::string>& arguments)
    : ExternalRunner(
        reactor,
        {
            scriptPath.string(),
            templatePath.string(),
            renderedPath.string()
        },
        HookOptions::RunOptions::DEFAULT
    )
{
}

IExternalRunner::Arguments
TemplaterRunner::SpecializeArguments(const IExternalRunner::Arguments& unspecialized) const
{
    std::vector<std::string> fullArgs;
    if (unspecialized.size() > 0)
    {
        fullArgs.emplace_back("--template-args");
        fullArgs.insert(fullArgs.end(),
                        unspecialized.begin(),
                        unspecialized.end());
    }
    return fullArgs;
}
