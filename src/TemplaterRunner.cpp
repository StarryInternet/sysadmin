
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
        CombineArguments (
            {
                scriptPath.string(),
                templatePath.string(),
                renderedPath.string()
            },
            arguments
        ),
        HookOptions::RunOptions::DEFAULT
    )
{
}

std::vector<std::string>
CombineArguments(const std::vector<std::string>& paths,
                 const std::vector<std::string>& arguments)
{
    std::vector<std::string> fullArgs;
    fullArgs.insert(fullArgs.end(),
                    paths.begin(),
                    paths.end());
    if (arguments.size() > 0)
    {
        fullArgs.emplace_back("--template-args");
        fullArgs.insert(fullArgs.end(),
                        arguments.begin(),
                        arguments.end());
    }
    fullArgs.emplace_back("--committed");
    return fullArgs;
}
