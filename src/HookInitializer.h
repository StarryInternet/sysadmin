
#pragma once

#include "IConfigurator.h"
#include "HookManager.h"
#include "HookConfig.h"
#include "HookUser.h"
#include "decibel/messaging/Reactor.h"

#include <memory>
#include <folly/futures/Future.h>

std::shared_ptr<HookManager> HookManagerFactory(FullHooksConfig config,
                                                decibel::messaging::Reactor* reactor,
                                                IConfigurator* sysadmin,
                                                size_t parallelHooks);

folly::Future<std::vector<ConfigPair::Key>> FulfillHooks(IConfigurator* sysadmin,
                                                         const HookUser::HookContainer& hooks);
