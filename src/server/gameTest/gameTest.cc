#include <gmock/gmock.h>
  using ::testing::Eq;
#include <gtest/gtest.h>
  using ::testing::Test;

#include <list>
#include "Config.h"
#include "DatabaseEnv.h"
#include "DatabaseLoader.h"
#include "Warden.h"
#include "WardenWin.h"
#include "WorldSocket.h"
#include "WorldSession.h"
#include <boost/asio/signal_set.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

namespace game
{
namespace testing
{
    class gameTest : public Test
    {
    protected:
        gameTest(){}
        ~gameTest(){}

        virtual void SetUp(){}
        virtual void TearDown(){}
    };

    TEST_F(gameTest, DatabaseLoader_AddDatabaseConfigFile)
    {
        int argc = 0;
        char** argv;
        auto configFile = fs::absolute("E:/Programs/TrinityWrathMod/worldserver.conf.test");
        std::string configError;
        sConfigMgr->LoadInitial(configFile.generic_string(), std::vector<std::string>(argv, argv + argc), configError);
        DatabaseLoader loader("server.worldserver", DatabaseLoader::DATABASE_NONE);
        loader
            .AddDatabase(CharacterDatabase, "Character");
        ASSERT_TRUE(loader.Load());
    }

    TEST_F(gameTest, WorldSession_InitializeSession)
    {
        int argc = 0;
        char** argv;
        auto configFile = fs::absolute("E:/Programs/TrinityWrathMod/worldserver.conf.test");
        std::string configError;
        sConfigMgr->LoadInitial(configFile.generic_string(), std::vector<std::string>(argv, argv + argc), configError);
        DatabaseLoader loader("server.worldserver", DatabaseLoader::DATABASE_NONE);
        loader
            .AddDatabase(CharacterDatabase, "Character");
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
        _worldSession->InitializeSession();

        ASSERT_THAT(id, Eq(_worldSession->GetAccountId()));
    }

} // namespace testing
} // namespace game
