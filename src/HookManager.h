
#pragma once

#include "HookConfig.h"
#include "IConfigurator.h"
#include "HookTable.h"
#include "HookUser.h"
#include "ConfigTypes.h"

#include "decibel/messaging/Reactor.h"

#include <memory>
#include <vector>

template <typename T>
folly::Future<folly::Unit> SplitSubprocessesAcrossCores(const T& subprocs,
                                                        size_t cores)
{
    std::vector<std::vector<std::shared_ptr<const HookUser>>> coreBuckets;
    coreBuckets.resize((subprocs.size() / cores) + (subprocs.size() % cores));
    size_t j = 0;
    for (auto s : subprocs)
    {
        size_t core_counter = (j / cores);
        coreBuckets.at(core_counter).push_back(s);
        j++;
    }
    auto final_future = folly::makeFuture();
    for (auto& b : coreBuckets)
    {
        final_future = final_future.then([b]
        {
            std::vector<folly::Future<folly::Unit>> futures;
            for (auto subproc : b)
            {
                futures.emplace_back(subproc->ExecuteExternalProcess());
            }
            return folly::collect(futures).then([]{});
        });
    }
    return final_future;
}


class HookManager
{
public:
    typedef std::pair<std::vector<std::shared_ptr<const HookUser>>,
                      std::vector<std::shared_ptr<const HookUser>>> HookDump;
    typedef HookTable::TemplatesSet TemplatesToRender;
    typedef HookTable::RunLevelServicesMap ServicesMap;


    struct HookPipeline
    {
        HookPipeline(TemplatesToRender& t, ServicesMap& s)
          : templates(t)
          , services(s)
          {
          };

        TemplatesToRender templates;
        ServicesMap services;
    };
public:
    HookManager(std::unique_ptr<HookTable> pTable,
                size_t parallelHooks);

    folly::Future<folly::Unit> HandleCommit(const ConfigPairList& committed,
                                            bool run_services = true);

    HookManager::HookPipeline GetHooksToRun(const ConfigPairList& committed,
                                            bool run_services = true);

    HookManager::HookPipeline GetAllHooks();

    folly::Future<folly::Unit> RunHooks(const HookManager::HookPipeline& hooks);

    HookManager::HookDump DumpHooks();

    folly::Future<bool> TriggerHook(const std::string& name);

    size_t NumberOfTemplaters() const;
    size_t NumberOfServices() const;

private:
    std::unique_ptr<HookTable> mpTable;
    size_t mParallelHooks;
};
