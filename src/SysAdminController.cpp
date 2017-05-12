#include "SysAdminController.h"
#include "ExternalRunner.h"
#include "Configurator.h"
#include "TransactionManager.h"
#include <algorithm>
#include <log4cxx/logger.h>

namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("SysAdminController"));
}

SysAdminController::SysAdminController(IConfigurator* configurator, ICommandInterface* command,
                                       std::shared_ptr<HookManager> hooks)
  : mTranslator()
  , mpConfigurator(configurator)
  , mpCommandInterface(command)
  , mpHookManager(hooks)
{

}

SysAdminController::~SysAdminController()
{

}

folly::Future<SysAdminController::ResponseType>
SysAdminController::GetHandler(sysadminctl::Command message, size_t)
{
    LOG4CXX_DEBUG(spLogger, "Handling get request: " << message.DebugString());
    auto maybeKey = mTranslator.TranslateToGet(message);
    if (!maybeKey)
    {
        LOG4CXX_ERROR(spLogger, "Could not translate get message");
        sysadminctl::Response output;
        output.set_status(sysadminctl::StatusCode::COMMAND_TRANSLATION_ERROR);
        output.mutable_get();
        return folly::makeFuture(boost::make_optional(output));
    }
    auto key = maybeKey.get();
    std::vector<ConfigPair> values;
    auto valueMap = mpConfigurator->Get(key);
    for (const auto& iter : valueMap)
    {
        values.push_back(iter.second);
    }
    auto getResp = mTranslator.TranslateFromGet(values);
    if (getResp.get().kvs_size() > 0)
    {
        getResp.set_status(sysadminctl::StatusCode::SUCCESS);
    }
    getResp.set_id(message.id());
    return folly::makeFuture(boost::make_optional(getResp));
}

folly::Future<SysAdminController::ResponseType>
SysAdminController::SetHandler(sysadminctl::Command message, size_t clientId)
{
    LOG4CXX_DEBUG(spLogger, "Handling set request: " << message.DebugString());
    sysadminctl::Response output;
    output.set_id(message.id());
    auto pair = mTranslator.TranslateToSet(message);
    if (!pair)
    {
        LOG4CXX_ERROR(spLogger, "Could not translate set message");
        output.set_status(sysadminctl::StatusCode::COMMAND_TRANSLATION_ERROR);
        return folly::makeFuture(boost::make_optional(output));
    }
    auto conf = pair.get();
    if (conf.GetKey().ToString().find("*") != std::string::npos)
    {
        LOG4CXX_ERROR(spLogger, "Invalid Key containing wildcard character");
        output.set_status(sysadminctl::StatusCode::INVALID_KEY);
        return folly::makeFuture(boost::make_optional(output));
    }
    if (conf.GetKey().SubkeyMatches(ConfigKey("last.*")))
    {
        LOG4CXX_ERROR(spLogger, "Users cannot set last.* namespace keys");
        output.set_status(sysadminctl::StatusCode::INVALID_KEY);
        return folly::makeFuture(boost::make_optional(output));
    }
    if (message.has_xid())
    {
        clientId = message.xid();
    }
    LOG4CXX_INFO(spLogger, "Handling set: " << conf.GetKey().ToString() << "=" <<
                 conf.GetValue().ToString() << ". XID: " << clientId);
    auto success = mpConfigurator->Set(conf, clientId);
    switch (success)
    {
        case IConfigurator::SUCCESS:
            output = mTranslator.TranslateFromGet({conf});
            output.set_id(message.id());
            output.set_status(sysadminctl::StatusCode::SUCCESS);
            break;
        case IConfigurator::SUCCESS_NEW_KEY:
            output = mTranslator.TranslateFromGet({conf});
            output.set_id(message.id());
            output.set_status(sysadminctl::StatusCode::SUCCESS_KEY_CREATED);
            break;
        case IConfigurator::TYPE_MISMATCH:
            output.set_status(sysadminctl::StatusCode::TYPE_MISMATCH);
            break;
        case IConfigurator::LOCKED_QUEUE:
            output.set_status(sysadminctl::StatusCode::LOCKED_QUEUE);
            break;
    }
    return folly::makeFuture(boost::make_optional(output));
}

folly::Future<SysAdminController::ResponseType>
SysAdminController::CommitHandler(sysadminctl::Command message, size_t clientId)
{
    sysadminctl::CommitConfig commit_opt = sysadminctl::CommitConfig::DEFAULT;
    if (message.commit().has_config())
    {
        commit_opt = message.commit().config();
    }
    if (message.has_xid())
    {
        clientId = message.xid();
    }
    LOG4CXX_INFO(spLogger, "Handling commit request. XID: " << clientId);
    sysadminctl::Response output;
    output.set_id(message.id());
    return mpConfigurator->Commit(clientId,
                                  commit_opt).then([output](CommitHistory::CommitId id) mutable
    {
        output.set_status(sysadminctl::StatusCode::SUCCESS);
        sysadminctl::CommitResponse* commitOutput = output.mutable_commit();
        commitOutput->set_commit_id(id);
        return boost::make_optional(output);
    }).onError([output](const CommitError& err) mutable
    {
        output.set_status(sysadminctl::StatusCode::EXTERNAL_PROCESS_ERROR);
        sysadminctl::CommitResponse* commitOutput = output.mutable_commit();
        commitOutput->set_commit_id(err.mCommitId);
        return boost::make_optional(output);
    });
}

folly::Future<SysAdminController::ResponseType>
SysAdminController::DropHandler(sysadminctl::Command message, size_t clientId)
{
    if (message.has_xid())
    {
        clientId = message.xid();
    }
    LOG4CXX_INFO(spLogger, "Handling drop request. XID: " << clientId);
    mpConfigurator->Drop(clientId);
    sysadminctl::Response output;
    output.set_id(message.id());
    output.set_status(sysadminctl::StatusCode::SUCCESS);
    return folly::makeFuture(boost::make_optional(output));
}

folly::Future<SysAdminController::ResponseType>
SysAdminController::FireHooksHandler(sysadminctl::Command message, size_t)
{
    LOG4CXX_INFO(spLogger, "Handling fire hooks request...");
    sysadminctl::Response output;
    output.set_id(message.id());
    return mpHookManager->RunHooks(mpHookManager->GetAllHooks()).then([output]() mutable
    {
        LOG4CXX_INFO(spLogger, "All hooks completed successfully");
        output.set_status(sysadminctl::StatusCode::SUCCESS);
        return folly::makeFuture(boost::make_optional(output));
    }).onError([output](const ExternalRunnerError& err) mutable
    {
        LOG4CXX_ERROR(spLogger, "Error while firing all hooks: " << err.what());
        output.set_status(sysadminctl::StatusCode::EXTERNAL_PROCESS_ERROR);
        return folly::makeFuture(boost::make_optional(output));
    });
}

folly::Future<SysAdminController::ResponseType>
SysAdminController::EraseHandler(sysadminctl::Command message, size_t clientId)
{
    LOG4CXX_DEBUG(spLogger, "Handling erase request: " << message.DebugString());
    sysadminctl::Response output;
    output.set_id(message.id());
    auto maybeKey = mTranslator.TranslateToErase(message);
    if (!maybeKey)
    {
        LOG4CXX_ERROR(spLogger, "Could not translate erase message");
        output.set_status(sysadminctl::StatusCode::COMMAND_TRANSLATION_ERROR);
        return folly::makeFuture(boost::make_optional(output));
    }
    auto key = maybeKey.get();
    if (message.has_xid())
    {
        clientId = message.xid();
    }
    LOG4CXX_INFO(spLogger, "Handling erase for key: " << key.ToString() << ". XID: " << clientId);
    mpConfigurator->Erase(key, clientId);
    output.set_status(sysadminctl::StatusCode::SUCCESS);
    return folly::makeFuture(boost::make_optional(output));
}

folly::Future<SysAdminController::ResponseType>
SysAdminController::RollbackHandler(sysadminctl::Command message, size_t)
{
    LOG4CXX_INFO(spLogger, "Handling rollback request: " << message.DebugString());
    sysadminctl::Response output;
    output.set_id(message.id());
    auto maybeId = mTranslator.TranslateToRollback(message);
    if (!maybeId)
    {
        LOG4CXX_ERROR(spLogger, "Could not translate rollback message");
        output.set_status(sysadminctl::StatusCode::COMMAND_TRANSLATION_ERROR);
        return folly::makeFuture(boost::make_optional(output));
    }
    CommitHistory::CommitId id = maybeId.get();

    return mpCommandInterface->Rollback(id).then([output]() mutable
    {
        output.set_status(sysadminctl::StatusCode::SUCCESS);
        return boost::make_optional(output);
    }).onError([output](const RollbackError& ) mutable
    {
        output.set_status(sysadminctl::StatusCode::FAILED_ROLLBACK);
        return boost::make_optional(output);
    }).onError([output](const ExternalRunnerError& ) mutable
    {
        output.set_status(sysadminctl::StatusCode::EXTERNAL_PROCESS_ERROR);
        return boost::make_optional(output);
    }).onError([output](const CommitError& ) mutable
    {
        output.set_status(sysadminctl::StatusCode::EXTERNAL_PROCESS_ERROR);
        return boost::make_optional(output);
    });
}

folly::Future<SysAdminController::ResponseType>
SysAdminController::ResetHandler(sysadminctl::Command message, size_t clientId)
{
    LOG4CXX_INFO(spLogger, "Handling reset request. XID: " << clientId);
    sysadminctl::Response output;
    output.set_id(message.id());
    mpConfigurator->Erase(ConfigKey("*"), clientId);

    auto keys = mpConfigurator->GetAllKeys(ConfigKey("default.*"));
    auto defaultValues = mpConfigurator->Get(ConfigKey("default.*"));
    for (const auto& kv : defaultValues)
    {
        ConfigKey new_key(kv.second.GetKey().Tail());
        auto status = mpConfigurator->Set(ConfigPair(new_key, kv.second.GetValue()),
                                                          clientId);
        if (status != IConfigurator::SUCCESS)
        {
            mpConfigurator->Drop(clientId);
            output.set_status(sysadminctl::StatusCode::TYPE_MISMATCH);
            return folly::makeFuture(boost::make_optional(output));
        }
    }

    return mpConfigurator->Commit(clientId).then([output](CommitHistory::CommitId id) mutable
    {
        output.set_status(sysadminctl::StatusCode::SUCCESS);
        sysadminctl::ResetResponse* resetOutput = output.mutable_reset();
        resetOutput->set_commit_id(id);
        return boost::make_optional(output);
    }).onError([output](const CommitError& err) mutable
    {
        output.set_status(sysadminctl::StatusCode::EXTERNAL_PROCESS_ERROR);
        sysadminctl::ResetResponse* resetOutput = output.mutable_reset();
        resetOutput->set_commit_id(err.mCommitId);
        return boost::make_optional(output);
    });
}

folly::Future<SysAdminController::ResponseType>
SysAdminController::DumpHooksHandler(sysadminctl::Command message, size_t clientId)
{
    LOG4CXX_INFO(spLogger, "Handling dump hooks request. XID: " << clientId);
    sysadminctl::Response output;
    sysadminctl::DumpResponse* dumpOutput = output.mutable_dump();
    output.set_id(message.id());
    HookManager::HookDump hookDump = mpHookManager->DumpHooks();
    std::for_each(hookDump.first.begin(),
                  hookDump.first.end(),
                  [dumpOutput](std::shared_ptr<const HookUser> hook)
    {
        dumpOutput->add_templatehooks(hook->Name());
    });
    std::for_each(hookDump.second.begin(),
                  hookDump.second.end(),
                  [dumpOutput](std::shared_ptr<const HookUser> hook)
    {
        dumpOutput->add_servicehooks(hook->Name());
    });
    output.set_status(sysadminctl::StatusCode::SUCCESS);
    auto response = boost::make_optional(output);
    return folly::makeFuture<SysAdminController::ResponseType>(std::move(response));
}

folly::Future<SysAdminController::ResponseType>
SysAdminController::TriggerHookHandler(sysadminctl::Command message, size_t clientId)
{
    LOG4CXX_INFO(spLogger, "Handling trigger hook request. XID: " << clientId);
    sysadminctl::Response output;
    output.set_id(message.id());
    sysadminctl::TriggerHook cmd = message.trigger();
    return mpHookManager->TriggerHook(cmd.hook())
        .then([output](bool found) mutable {
            if (found)
            {
                LOG4CXX_INFO(spLogger, "Hook successfully triggered");
                output.set_status(sysadminctl::StatusCode::SUCCESS);
            }
            else
            {
                LOG4CXX_ERROR(spLogger, "Could not find requested hook");
                output.set_status(sysadminctl::StatusCode::HOOK_NOT_FOUND);
            }
            return boost::make_optional(output);
        })
        .onError([output](const ExternalRunnerError& err) mutable {
            LOG4CXX_ERROR(spLogger, "Caught ExternalRunner exception: " << err.what());
            output.set_status(sysadminctl::StatusCode::EXTERNAL_PROCESS_ERROR);
            return boost::make_optional(output);
        });
}

folly::Future<SysAdminController::ResponseType>
SysAdminController::BlameHandler(sysadminctl::Command message, size_t clientId)
{
    LOG4CXX_INFO(spLogger, "Handling blame request. XID: " << clientId);
    auto maybeKey = mTranslator.TranslateToBlame(message);
    if (!maybeKey)
    {
        LOG4CXX_ERROR(spLogger, "Could not translate blame message");
        sysadminctl::Response output;
        output.set_status(sysadminctl::StatusCode::COMMAND_TRANSLATION_ERROR);
        output.mutable_blame();
        return folly::makeFuture(boost::make_optional(output));
    }

    std::vector<BlameDelta> deltas = mpCommandInterface->Blame(maybeKey.get());
    auto blameResp = mTranslator.TranslateFromBlame(maybeKey.get(), deltas);

    //If the key doesn't currently exist and there is no history of the key existing,
    //set status to KEY_NOT_FOUND
    if (mpConfigurator->Get(maybeKey.get()).size() == 0 && blameResp.blame().entries_size() == 0)
    {
        blameResp.set_status(sysadminctl::StatusCode::KEY_NOT_FOUND);
    }
    else
    {
        blameResp.set_status(sysadminctl::StatusCode::SUCCESS);
    }
    blameResp.set_id(message.id());
    return folly::makeFuture(boost::make_optional(blameResp));
}

folly::Future<SysAdminController::ResponseType>
SysAdminController::InFlightHandler(sysadminctl::Command message, size_t clientId)
{
    if (message.has_xid())
    {
        clientId = message.xid();
    }
    LOG4CXX_INFO(spLogger, "Handling Get In Flight items request. XID: " << clientId);

    auto inflight = mpConfigurator->InFlightItems(clientId);
    auto resp = mTranslator.TranslateFromInFlight(inflight);
    resp.set_id(message.id());
    resp.set_status(sysadminctl::StatusCode::SUCCESS);

    return folly::makeFuture(boost::make_optional(resp));
}
