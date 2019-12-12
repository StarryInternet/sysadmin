
#include "HookManager.h"
#include "ExternalRunner.h"
#include "HookTable.h"
#include "HookUser.h"

#include <boost/assert.hpp>
#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <log4cxx/logger.h>
#include <folly/futures/Promise.h>
#include <memory>


namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("HookManager"));
}

namespace dm = decibel::messaging;

HookManager::HookManager(std::unique_ptr<HookTable> pTable,
                         size_t parallelHooks)
  : mpTable(std::move(pTable))
  , mParallelHooks(parallelHooks)
{
}

folly::Future<folly::Unit> HookManager::HandleCommit(const ConfigPairList& committed,
                                                     bool run_services)
{
    auto hooks = GetHooksToRun(committed, run_services);
    return RunHooks(hooks);
}

folly::Future<folly::Unit> HookManager::RunHooks(const HookManager::HookPipeline& hooks)
{
    return SplitSubprocessesAcrossCores(hooks.templates, mParallelHooks).thenValue([this, hooks](auto /*unused*/)
    {
        LOG4CXX_INFO(spLogger, "Templates rendered and dumped, running services");
        auto final_future = folly::makeFuture();
        for (auto rlPair : hooks.services)
        {
            final_future = std::move(final_future).thenValue([this, rlPair](auto /*unused*/)
            {
                return SplitSubprocessesAcrossCores(rlPair.second, mParallelHooks);
            });
        }
        return final_future;
    }).thenError(folly::tag_t<ExternalRunnerError>{}, [](const auto& err)
    {
        LOG4CXX_ERROR(spLogger, "Templater failed while running: " << err.what());
        throw err;
    });
}

HookManager::HookPipeline HookManager::GetAllHooks()
{
    TemplatesToRender templatesToRender;
    ServicesMap servicesToRun = mpTable->GetOrderedServices();

    for (auto templater: mpTable->ByType(HookType::TEMPLATER))
    {
        templatesToRender.emplace(templater);
    }
    return HookManager::HookPipeline(templatesToRender, servicesToRun);
}

HookManager::HookPipeline
HookManager::GetHooksToRun(const ConfigPairList& committed, bool run_services)
{
    TemplatesToRender templatesToRender;
    ServicesMap servicesToRun;
    ServicesMap allServices = mpTable->GetOrderedServices();

    for (const auto& pair : committed)
    {
        for (auto templater: mpTable->ByType(HookType::TEMPLATER))
        {
            if (templater->HasHook(pair.GetKey()))
            {
                templatesToRender.emplace(templater);
            }
        }
        if (run_services)
        {
            for (auto service: mpTable->RunLevelOrder(HookType::SERVICE))
            {
                if (service->HasHook(pair.GetKey()))
                {
                    size_t rl = service->RunLevel();
                    if (!servicesToRun.count(rl))
                    {
                        servicesToRun.emplace(rl, std::vector<std::shared_ptr<const HookUser>>());
                    }
                    if (std::find(servicesToRun.at(rl).begin(),
                                  servicesToRun.at(rl).end(),
                                  service) == servicesToRun.at(rl).end())
                    {
                        servicesToRun.at(rl).emplace_back(service);
                    }
                }
            }
        }
    }
    return HookManager::HookPipeline(templatesToRender, servicesToRun);
}

HookManager::HookDump HookManager::DumpHooks()
{
    HookManager::HookDump dump;
    dump.first = mpTable->ByType(HookType::TEMPLATER);
    dump.second = mpTable->ByType(HookType::SERVICE);
    return dump;
}

folly::Future<bool> HookManager::TriggerHook(const std::string& name)
{
    auto hook = mpTable->ByName(name);
    if (hook)
    {
        TemplatesToRender t;
        ServicesMap s;
        if ((*hook)->GetType() == HookType::TEMPLATER)
        {
            t.emplace(hook.get());
        }
        else
        {
            // run level doesnt matter here, since we are only running a single service
            s.emplace(1, std::vector<std::shared_ptr<const HookUser>>());
            s.at(1).emplace_back(hook.get());
        }
        return RunHooks(HookManager::HookPipeline(t, s)).thenValue([](auto /*unused*/) {return true; });
    }
    else
    {
        return folly::makeFuture(false);
    }
}

size_t HookManager::NumberOfTemplaters() const
{
    return mpTable->ByType(HookType::TEMPLATER).size();
}

size_t HookManager::NumberOfServices() const
{
    return mpTable->ByType(HookType::SERVICE).size();
}
