
#pragma once

#include "PassThroughConfigurator.h"

class WildcardConfigurator : public PassThroughConfigurator
{
public:
    WildcardConfigurator(IConfigurator* storage);

    virtual ~WildcardConfigurator();

public: // PassThroughConfigurator
    virtual ConfigPairMap Get(const ConfigPair::Key& key) const;

    virtual void Erase(const ConfigPair::Key& key, IConfigurator::ClientId clientId);

private:
    IConfigurator* mBackingStore;
};
