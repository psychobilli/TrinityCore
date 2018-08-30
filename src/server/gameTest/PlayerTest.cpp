#include <gmock/gmock.h>
using ::testing::Eq;
#include <gtest/gtest.h>
using ::testing::Test;

#include "Player.h"
#include "CharacterHandler.cpp"
#include "Config.h"
#include "DatabaseEnv.h"
#include "DatabaseLoader.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "../scripts/ScriptLoader.h"
#include "SQLOperation.h"
#include "WorldSocket.h"
#include "WorldSession.h"
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

TEST(Player, LoadPlayer)
{
    int argc = 0;
    char** argv;
    auto configFile = fs::absolute("E:/Programs/TrinityWrathMod/worldserver.conf.test");
    std::string configError;
    sConfigMgr->LoadInitial(configFile.generic_string(), std::vector<std::string>(argv, argv + argc), configError);
    DatabaseLoader loader("server.worldserver", DatabaseLoader::DATABASE_NONE);
    loader
        .AddDatabase(CharacterDatabase, "Character")
        .AddDatabase(LoginDatabase, "Login")
        .AddDatabase(WorldDatabase, "World");
    ASSERT_TRUE(loader.Load());

    uint32 id = 1;
    std::shared_ptr<AuthSession> authSession;
    std::shared_ptr<WorldSocket> worldSocket;
    PreparedQueryResult result;
    int64 mutetime = 0;
    int8 locale = 0;
    WorldSession* _worldSession;
    _worldSession = new WorldSession(id, "ADMIN", worldSocket, AccountTypes(0),
        2, mutetime, LOCALE_enUS, 0, false);

    sScriptMgr->SetScriptLoader(AddScripts);
    sWorld->SetInitialWorldSettings();

    Player* p = new Player(_worldSession);
    ObjectGuid playerGuid;
    playerGuid.Set(1);
    LoginQueryHolder *holder = new LoginQueryHolder(id, playerGuid);
    holder->Initialize();
    QueryResultHolderFuture _charLoginCallback = CharacterDatabase.DelayQueryHolder(holder);
    
    ASSERT_TRUE(p->LoadFromDB(playerGuid, reinterpret_cast<LoginQueryHolder*>(_charLoginCallback.get())));
    ASSERT_EQ("Ineur", p->GetName());
}
