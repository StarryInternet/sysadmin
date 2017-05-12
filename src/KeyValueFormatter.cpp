
#include "KeyValueFormatter.h"
#include <log4cxx/logger.h>

#include <algorithm>
#include <fstream>
#include <string>

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("TemplateRunner"));
}

std::vector<std::string> FormatKeys(const ConfigPairList& values)
{
    std::vector<std::string> output;
    for (auto iter = values.begin(); iter != values.end(); iter++)
    {
        output.push_back(iter->GetKey().ToString());
    }
    return output;
}
