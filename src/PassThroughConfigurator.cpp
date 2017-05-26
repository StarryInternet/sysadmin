
#include "PassThroughConfigurator.h"

#include <boost/assert.hpp>

namespace dm = decibel::messaging;

PassThroughConfigurator::PassThroughConfigurator(IConfigurator* backingStore)
  : mBackingStore(backingStore)
{
}

PassThroughConfigurator::~PassThroughConfigurator()
{
}

IConfigurator::SetStatus PassThroughConfigurator::Set(const ConfigPair& value, dm::Protocol::Id id)
{
    return mBackingStore->Set(value, id);
}

ConfigPairMap PassThroughConfigurator::Get(const ConfigPair::Key& key) const
{
    return mBackingStore->Get(key);
}

bool PassThroughConfigurator::TypeCheck(const ConfigPair& comp) const
{
    return mBackingStore->TypeCheck(comp);
}

ConfigPairMap PassThroughConfigurator::GetAll() const
{
    return mBackingStore->GetAll();
}

std::vector<ConfigPair::Key> PassThroughConfigurator::GetAllKeys(const ConfigPair::Key& ns) const
{
    BOOST_ASSERT(ns.IsWildcard());
    return mBackingStore->GetAllKeys(ns);
}

folly::Future<CommitHistory::CommitId>
PassThroughConfigurator::Commit(size_t id, sysadminctl::CommitConfig)
{
    return mBackingStore->Commit(id);
}

void PassThroughConfigurator::Drop(size_t id)
{
    mBackingStore->Drop(id);
}

void PassThroughConfigurator::Erase(const ConfigPair::Key& key, size_t id)
{
    mBackingStore->Erase(key, id);
}

ConfigPairList PassThroughConfigurator::InFlightItems(size_t id)
{
    return mBackingStore->InFlightItems(id);
}
