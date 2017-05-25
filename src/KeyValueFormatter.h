
#pragma once

#include "ConfigTypes.h"
#include "IExternalRunner.h"

#include <vector>

IExternalRunner::Arguments FormatKeys(const ConfigPairList& values);

std::string UnderscoreKey(const std::string& key);
