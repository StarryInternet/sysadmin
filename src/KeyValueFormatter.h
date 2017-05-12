
#pragma once

#include "ConfigTypes.h"
#include "IExternalRunner.h"

#include <vector>

IExternalRunner::FormattedValues FormatKeys(const ConfigPairList& values);

std::string UnderscoreKey(const std::string& key);
