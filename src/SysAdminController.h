
#pragma once

#include <folly/futures/Future.h>
#include <memory>

#include "ProtobufTranslator.h"
#include "ConfigTypes.h"
#include "IConfigurator.h"
#include "ICommandInterface.h"
#include "HookManager.h"

#include "sysadmin-api/sysadminctl.pb.h"

class SysAdminController
{
public:
    typedef boost::optional<sysadminctl::Response> ResponseType;

    SysAdminController(IConfigurator* configurator, ICommandInterface* command,
                       std::shared_ptr<HookManager> hooks);

    ~SysAdminController();

    folly::Future<ResponseType> GetHandler(sysadminctl::Command message, size_t clientId);
    folly::Future<ResponseType> SetHandler(sysadminctl::Command message, size_t clientId);
    folly::Future<ResponseType> CommitHandler(sysadminctl::Command message, size_t clientId);
    folly::Future<ResponseType> DropHandler(sysadminctl::Command message, size_t clientId);
    folly::Future<ResponseType> FireHooksHandler(sysadminctl::Command message, size_t clientId);
    folly::Future<ResponseType> EraseHandler(sysadminctl::Command message, size_t clientId);
    folly::Future<ResponseType> RollbackHandler(sysadminctl::Command message, size_t clientId);
    folly::Future<ResponseType> ResetHandler(sysadminctl::Command message, size_t clientId);
    folly::Future<ResponseType> DumpHooksHandler(sysadminctl::Command message, size_t clientId);
    folly::Future<ResponseType> TriggerHookHandler(sysadminctl::Command message, size_t clientId);
    folly::Future<ResponseType> BlameHandler(sysadminctl::Command message, size_t clientId);
    folly::Future<ResponseType> InFlightHandler(sysadminctl::Command message, size_t clientId);

private:
    ProtobufTranslator mTranslator;
    IConfigurator* mpConfigurator;
    ICommandInterface* mpCommandInterface;
    std::shared_ptr<HookManager> mpHookManager;
};
