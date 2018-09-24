#include <gmock/gmock.h>
  using ::testing::Eq;
#include <gtest/gtest.h>
  using ::testing::Test;

#include <list>
#include "Config.h"
#include "DatabaseEnv.h"
#include "DatabaseLoader.h"
#include "WorldSocket.h"
#include "WorldSession.h"
#include <boost/asio/signal_set.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

class gameTest : public Test
{
protected:
    gameTest(){}
    ~gameTest(){}

    virtual void SetUp(){}
    virtual void TearDown(){}
};

TEST(DatabaseLoader, AddDatabaseConfigFile)
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
