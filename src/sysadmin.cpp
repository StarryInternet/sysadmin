#include <iostream>

#ifdef _SYSADMIN_USE_SD_NOTIFY
#include <systemd/sd-daemon.h>
#endif

#include <boost/filesystem.hpp>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

#include "decibel/Config.h"
#include "decibel/messaging/Reactor.h"
#include "decibel/messaging/ProtobufProtocolFactory.h"
#include "decibel/Protobuf.h"

#include "sysadmin-api/sysadminctl.pb.h"
#include "Configurator.h"
#include "LocalYAMLStorage.h"
#include "SysAdminConfig.h"
#include "SysAdminController.h"
#include "HookConfig.h"
#include "HookInitializer.h"
#include "MemoryCache.h"
#include "LocalHistoryStorage.h"
#include "CommitHistory.h"

namespace dm = decibel::messaging;
using namespace std::placeholders;
namespace bfs = boost::filesystem;


namespace
{
    log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("sysadmin"));
}

int main(int argc, const char* argv[])
{
    SysAdminConfig config;
    try
    {
        config = decibel::LoadConfig<SysAdminConfig>(argc, argv);
    }
    catch (const decibel::LoadConfigError& e)
    {
        std::clog << "Error loading config: " << e.what() << std::endl;
        return -1;
    }
    log4cxx::PropertyConfigurator::configure(config.log4cxxConfig);

    LocalYAMLStorage storage(config.storagePath);
    MemoryCache cache(&storage);
    LocalHistoryStorage history(config.historyStorage, YAMLSerializer, YAMLDeserializer);
    Configurator configurator(&cache, &history);

    dm::Reactor reactor;

    FullHooksConfig hookConf;
    std::shared_ptr<HookManager> hookMgmr;
    if (config.hookConfig != "")
    {
        if (!boost::filesystem::exists(config.hookConfig))
        {
            LOG4CXX_ERROR(spLogger, "Path does not exist: " << config.hookConfig);
            return -1;
        }

        if (!boost::filesystem::is_regular_file(config.hookConfig))
        {
            LOG4CXX_ERROR(spLogger, "Invalid file type");
            return -1;
        }

        auto node = YAML::LoadFile(config.hookConfig);
        hookConf = node.as<FullHooksConfig>();
        LOG4CXX_INFO(spLogger, "Loading hooks...");
        hookMgmr = HookManagerFactory(hookConf, &reactor, &configurator, config.parallelHooks);
        configurator.RegisterCommitHook(std::bind(&HookManager::HandleCommit, hookMgmr, _1, _2));
    }
    else
    {
        LOG4CXX_INFO(spLogger, "No hook config provided, no hooks will be serviced");
    }
    SysAdminController controller(&configurator, &configurator, hookMgmr);

    decibel::OneofDispatcher<sysadminctl::Command, SysAdminController::ResponseType, dm::Protocol::Id> mDispatcher;
    mDispatcher.RegisterHandler("set", std::bind(&SysAdminController::SetHandler, &controller, _1, _2));
    mDispatcher.RegisterHandler("get", std::bind(&SysAdminController::GetHandler, &controller, _1, _2));
    mDispatcher.RegisterHandler("commit", std::bind(&SysAdminController::CommitHandler, &controller, _1, _2));
    mDispatcher.RegisterHandler("drop", std::bind(&SysAdminController::DropHandler, &controller, _1, _2));
    mDispatcher.RegisterHandler("firehooks", std::bind(&SysAdminController::FireHooksHandler, &controller, _1, _2));
    mDispatcher.RegisterHandler("erase", std::bind(&SysAdminController::EraseHandler, &controller, _1, _2));
    mDispatcher.RegisterHandler("rollback", std::bind(&SysAdminController::RollbackHandler, &controller, _1, _2));
    mDispatcher.RegisterHandler("reset", std::bind(&SysAdminController::ResetHandler, &controller, _1, _2));
    mDispatcher.RegisterHandler("dump", std::bind(&SysAdminController::DumpHooksHandler, &controller, _1, _2));
    mDispatcher.RegisterHandler("trigger", std::bind(&SysAdminController::TriggerHookHandler, &controller, _1, _2));
    mDispatcher.RegisterHandler("blame", std::bind(&SysAdminController::BlameHandler, &controller, _1, _2));
    mDispatcher.RegisterHandler("inflight", std::bind(&SysAdminController::InFlightHandler, &controller, _1, _2));

    dm::ProtobufProtocolFactory<sysadminctl::Command, sysadminctl::Response, uint32_t> factory(mDispatcher);
    factory.RegisterSendErrorCallback(
        [](dm::ProtobufOneofProtocol<sysadminctl::Command, sysadminctl::Response, uint32_t>* protocol,
           const sysadminctl::Response& resp)
        {
            sysadminctl::Response errResponse;
            errResponse.set_id(resp.id());
            errResponse.set_status(sysadminctl::StatusCode::MESSAGE_SIZE_ERROR);
            protocol->Send(errResponse);
        });

#ifdef _SYSADMIN_USE_SD_NOTIFY
    reactor.add([]() {
        auto rc = sd_notify(1, "READY=1");

        if (rc < 0) {
            LOG4CXX_WARN(spLogger, "Failed to notify systemd, error: " << rc);
        }
    });
#endif

    LOG4CXX_INFO(spLogger, "Starting sysadmin on port " << config.port);
    reactor.ServeTcp(config.host, config.port, &factory);
    reactor.Start();
}
