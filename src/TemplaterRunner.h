
#pragma once

#include "ExternalRunner.h"

std::vector<std::string>
CombineArguments(const std::vector<std::string>& paths,
                 const std::vector<std::string>& arguments);

class TemplaterRunner : public ExternalRunner
{
public:
    TemplaterRunner(decibel::messaging::Reactor* reactor,
                    const boost::filesystem::path& scriptPath,
                    const boost::filesystem::path& templatePath,
                    const boost::filesystem::path& renderedPath,
                    const std::vector<std::string>& arguments);

    virtual IExternalRunner::Arguments
    SpecializeArguments(const IExternalRunner::Arguments& unspecialized) const;
};
