
#include "WildcardConfigurator.h"

#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("WildcardConfigurator"));
}

namespace dm = decibel::messaging;

WildcardConfigurator::WildcardConfigurator(IConfigurator* backingStore)
  : PassThroughConfigurator(backingStore)
  , mBackingStore(backingStore)
{
}

WildcardConfigurator::~WildcardConfigurator()
{
}

ConfigPairMap WildcardConfigurator::Get(const ConfigPair::Key& key) const
{
    ConfigPairMap output;
    if (key.IsWildcard())
    {
        auto subKeys = mBackingStore->GetAllKeys(key);
        for (const auto& subkey : subKeys)
        {
            auto res = mBackingStore->Get(subkey);
            if (res.count(subkey))
            {
                output.emplace(subkey, res.at(subkey));
            }
        }
    }
    else
    {
        auto res = mBackingStore->Get(key);
        if (res.count(key))
        {
            output.emplace(key, res.at(key));
        }
    }

    return output;
}

void WildcardConfigurator::Erase(const ConfigPair::Key& key, size_t id)
{
    if (key.IsWildcard())
    {
        auto subkeys = mBackingStore->GetAllKeys(key);
        for (const auto& key : subkeys)
        {
            // Don't allow wildcard deletion of last.* or default.* keys
            if (key.SubkeyMatches(ConfigKey("last.*")) ||
                key.SubkeyMatches(ConfigKey("default.*")))
            {
                continue;
            }
            LOG4CXX_INFO(spLogger, "Queueing up erase of subkey: " << key.ToString());
            mBackingStore->Erase(key, id);
        }
    }
    else
    {
        LOG4CXX_INFO(spLogger, "Queueing up erase of subkey: " << key.ToString());
        mBackingStore->Erase(key, id);
    }
}
