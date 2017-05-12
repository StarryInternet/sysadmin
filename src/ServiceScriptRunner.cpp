
#include "ServiceScriptRunner.h"

#include <folly/futures/Promise.h>

#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("ServiceScriptRunner"));
}

namespace bfs = boost::filesystem;
namespace dm = decibel::messaging;

ServiceScriptRunner::ServiceScriptRunner(dm::Reactor* reactor,
                                         const std::vector<std::string>& scriptPath,
                                         HookOptions::RunOptions options)
  : ExternalRunner(reactor, scriptPath, options)
{
}
