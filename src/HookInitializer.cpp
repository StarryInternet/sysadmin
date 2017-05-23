
#include "HookInitializer.h"
#include "HookTable.h"
#include "HookUser.h"

#include "TemplaterRunner.h"
#include "ServiceScriptRunner.h"

#include <algorithm>
#include <log4cxx/logger.h>
#include <memory>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("HookInitializer"));
}

namespace dm = decibel::messaging;

namespace sp = std::placeholders;

std::shared_ptr<HookManager> HookManagerFactory(FullHooksConfig config,
                                                dm::Reactor* reactor,
                                                IConfigurator* sysadmin,
                                                size_t parallelHooks)
{
    std::unique_ptr<HookTable> pTable(new HookTable);

    // TODO Realistically, there should be no internal difference between templates and services.
    // Templates should probably all be run first, so you could service the existing structure
    // of the hooks.yaml file by just having all templates be runlevel 0
    for (const auto& templateConfig : config.mTemplates)
    {
        auto templaterRunner = std::make_shared<TemplaterRunner>(reactor, config.mTemplaterPath,
                                                                 templateConfig.mPath,
                                                                 templateConfig.mDestination,
                                                                 templateConfig.mArguments);
        pTable->Insert(HookUser(templateConfig.mName,
                                HookType::TEMPLATER,
                                templateConfig.mHooks,
                                std::bind(&FulfillHooks, sysadmin, sp::_1),
                                templaterRunner,
                                1));
    }

    for (const auto& serviceConfig : config.mServices)
    {
        std::vector<std::string> fullPath = serviceConfig.mArguments;
        fullPath.insert(fullPath.begin(), serviceConfig.mPath);
        auto servicesRunner = std::make_shared<ServiceScriptRunner>(reactor,
                                                                    fullPath,
                                                                    serviceConfig.mRunOptions);
        pTable->Insert(HookUser(serviceConfig.mName,
                                HookType::SERVICE,
                                serviceConfig.mHooks,
                                std::bind(&FulfillHooks, sysadmin, sp::_1),
                                servicesRunner,
                                serviceConfig.mRunLevel));
    }

    return std::make_shared<HookManager>(std::move(pTable), parallelHooks);
}

folly::Future<std::vector<ConfigPair::Key>> FulfillHooks(IConfigurator* sysadmin,
                                                         const HookUser::HookContainer& hooks)
{
    std::vector<ConfigPair::Key> output;
    for (const auto& hook : hooks)
    {
        if (hook.mKey.IsWildcard())
        {
            output.push_back(ConfigPair::Key(hook.mKey));
        }
        else
        {
            auto pair = UnwrapSinglePair(sysadmin->Get(hook.mKey));
            if (!pair && !hook.mOptional)
            {
                return folly::makeFuture<std::vector<ConfigPair::Key>>(HookUserError("Couldn't satisfy hook: " + hook.mKey.ToString()));
            }
            else if (pair)
            {
                output.push_back(pair.get().GetKey());
            }
        }
    }
    return folly::makeFuture(std::move(output));
}
