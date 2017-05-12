
#pragma once

#include "ExternalRunner.h"

#include "decibel/messaging/Reactor.h"
#include "decibel/niceuv/FutureSubprocess.h"
#include <boost/filesystem.hpp>

class ServiceScriptRunner : public ExternalRunner
{
public:
    ServiceScriptRunner(decibel::messaging::Reactor* reactor,
                        const std::vector<std::string>& scriptPath,
                        HookOptions::RunOptions options=HookOptions::RunOptions::DEFAULT);
};
