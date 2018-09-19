
#include "Common.h"
#include "AppenderDB.h"
#include "AsyncAcceptor.h"
#include "Banner.h"
#include "BattlegroundMgr.h"
#include "BigNumber.h"
#include "../worldserver/CommandLine/CliRunnable.h"
#include "Configuration/Config.h"
#include "DatabaseEnv.h"
#include "DatabaseLoader.h"
#include "GitRevision.h"
#include "InstanceSaveMgr.h"
#include "IoContext.h"
#include "MapManager.h"
#include "Metric.h"
#include "MoqAsyncAcceptor.h"
#include "MySQLThreading.h"
#include "ObjectAccessor.h"
#include "OpenSSLCrypto.h"
#include "OutdoorPvP/OutdoorPvPMgr.h"
#include "ProcessPriority.h"
#include "../worldserver/RemoteAccess/RASession.h"
#include "RealmList.h"
#include "Resolver.h"
#include "ScriptLoader.h"
#include "ScriptMgr.h"
#include "ScriptReloadMgr.h"
#include "../worldserver/TCSoap/TCSoap.h"
#include "World.h"
#include "WorldSocket.h"
#include "WorldSocketMgr.h"
#include <openssl/opensslv.h>
#include <openssl/crypto.h>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>
#include <csignal>
#include <iostream>

using namespace boost::program_options;
namespace fs = boost::filesystem;

#ifndef _TRINITY_CORE_CONFIG
#define _TRINITY_CORE_CONFIG  "worldserver.conf"
#endif

#define WORLD_SLEEP_CONST 50

#ifdef _WIN32
#include "ServiceWin32.h"
#endif
static class main_moq {
public:
    static void SignalHandler(boost::system::error_code const& error, int signalNumber);
    static MoqAsyncAcceptor* StartRaSocketAcceptor(Trinity::Asio::IoContext& ioContext);
    static bool StartDB();
    static void StopDB();
    static void WorldUpdateLoop();
    static void ClearOnlineAccounts();
    static void ShutdownCLIThread(std::thread* cliThread);
    static bool LoadRealmInfo(Trinity::Asio::IoContext& ioContext);
    static variables_map GetConsoleArguments(int argc, char** argv, fs::path& configFile, std::string& cfg_service);
    static int main(int argc, char** argv);
    static int Shutdown();
    static std::shared_ptr<WorldSocket> GetWorldSocket();
};
