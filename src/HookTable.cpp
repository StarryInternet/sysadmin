#include "HookTable.h"

#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <memory>
#include <vector>

#include <log4cxx/logger.h>

#include "HookUser.h"
#include "HookManager.h"


namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("HookTable"));
}

HookTable::HookTable()
    : mTable()
{
}

void HookTable::Insert(HookUser&& hook)
{
    auto pHook = std::make_shared<HookUser>(std::move(hook));
    auto inserted = mTable.insert(pHook);
    if (!inserted.second)
    {
        LOG4CXX_WARN(spLogger, "Failed to insert hook with the same name as an existing hook. " <<
                     "Consider renaming hook: " << hook.Name());
    }
}

boost::optional<std::shared_ptr<const HookUser>>
HookTable::ByName(const std::string& name)
{
    auto found = mTable.get<Name>().find(name);
    if (found != mTable.get<Name>().end())
    {
        std::shared_ptr<const HookUser> convert = *found;
        return boost::make_optional(convert);
    }
    else
    {
        return boost::none;
    }
}

std::vector<std::shared_ptr<const HookUser>>
HookTable::ByType(const HookType type)
{
    auto found = mTable.get<Type>().equal_range(type);
    std::vector<std::shared_ptr<const HookUser>>
        byType(found.first, found.second);
    return byType;
}

HookTable::RunLevelServicesMap HookTable::GetOrderedServices()
{
    RunLevelServicesMap s;
    for (auto service: RunLevelOrder(HookType::SERVICE))
    {
        size_t rl = service->RunLevel();
        if (!s.count(rl))
        {
            s.emplace(rl, std::vector<std::shared_ptr<const HookUser>>());
        }
        if (std::find(s.at(rl).begin(),
                      s.at(rl).end(),
                      service) == s.at(rl).end())
        {
            s.at(rl).emplace_back(service);
        }
    }
    return s;
}

std::vector<std::shared_ptr<const HookUser>>
HookTable::RunLevelOrder(const HookType type)
{
    /*
     * Only specifying the first component of a composite key _should_ give us
     * all entries of that first component, sorted by the second component.
     */
    auto found = mTable.get<RunLevel>().equal_range(type);
    std::vector<std::shared_ptr<const HookUser>>
        sorted(found.first, found.second);
    return sorted;
}
