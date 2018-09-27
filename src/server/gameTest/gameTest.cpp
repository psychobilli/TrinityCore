#include <gmock/gmock.h>
  using ::testing::Eq;
#include <gtest/gtest.h>
  using ::testing::Test;

#include "Config.h"
#include "CharacterHandler.cpp"
#include "DatabaseEnv.h"
#include "DatabaseLoader.h"
#include "WorldServerMoq.h"
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

TEST(Player, LoadPlayer)
{
    int argc = 0;
    char **argv = new char*[0];

    WorldServerMoq* server = new WorldServerMoq();
    server->LaunchServer(argc, argv);

    uint32 id = 1;
    std::shared_ptr<AuthSession> authSession;
    std::shared_ptr<WorldSocket> worldSocket;
    PreparedQueryResult result;
    int64 mutetime = 0;
    int8 locale = 0;
    WorldSession* _worldSession;
    _worldSession = new WorldSession(id, "ADMIN", worldSocket, AccountTypes(0),
        2, mutetime, LOCALE_enUS, 0, false);

    Player* p = new Player(_worldSession);
    ObjectGuid playerGuid;
    playerGuid.Set(1);
    LoginQueryHolder *holder = new LoginQueryHolder(id, playerGuid);
    holder->Initialize();
    QueryResultHolderFuture _charLoginCallback = CharacterDatabase.DelayQueryHolder(holder);

    ASSERT_TRUE(p->LoadFromDB(playerGuid, reinterpret_cast<LoginQueryHolder*>(_charLoginCallback.get())));
    ASSERT_EQ("Ineur", p->GetName());
}

TEST(WorldSession, InitializeSession)
{
    int argc = 0;
    char** argv = new char*[0];
    WorldServerMoq* server = new WorldServerMoq();
    server->LaunchServer(argc, argv);

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

TEST(WorldSession, HandleMoveTeleportAck)
{
    // Arrange
    int argc = 0;
    char** argv = new char*[0];
    WorldServerMoq* server = new WorldServerMoq();
    server->LaunchServer(argc, argv);
    uint32 id = 1;
    std::shared_ptr<AuthSession> authSession;
    std::shared_ptr<WorldSocket> worldSocket;
    PreparedQueryResult result;
    int64 mutetime = 0;
    int8 locale = 0;
    WorldSession* _worldSession;
    _worldSession = new WorldSession(id, "ADMIN", worldSocket, AccountTypes(0),
        2, mutetime, LOCALE_enUS, 0, false);

    Player* p = new Player(_worldSession);
    ObjectGuid playerGuid;
    playerGuid.Set(1);
    LoginQueryHolder *holder = new LoginQueryHolder(id, playerGuid);
    holder->Initialize();
    QueryResultHolderFuture _charLoginCallback = CharacterDatabase.DelayQueryHolder(holder);
    p->LoadFromDB(playerGuid, reinterpret_cast<LoginQueryHolder*>(_charLoginCallback.get()));
    p->SetMovedUnit(p);
    p->SetSemaphoreTeleportNear(true);
    bool toNorthrend = false;
    if (p->GetMap()->GetId() == 571) {
        p->TeleportTo(608, 1808.819946, 803.929993, 44.363998, 6.282000);
    }
    else {
        toNorthrend = true;
        p->TeleportTo(571, 5680.7, 487.323, 652.418, 0.882);
    }

    _worldSession->InitializeSession();
    WorldPacket packet(MSG_MOVE_TELEPORT_ACK, 41);
    packet.appendPackGUID(10);
    packet << uint32(0);
    packet << uint32(1);
    WorldPacket *packetRef = &packet;

    // Act
    _worldSession->HandleMoveTeleportAck(*packetRef);

    // Assert
}

TEST(WorldSession, HandleMoveWorldportAckCode)
{
    // Arrange
    int argc = 0;
    char** argv = new char*[0];
    WorldServerMoq* server = new WorldServerMoq();
    server->LaunchServer(argc, argv);
    uint32 id = 1;
    std::shared_ptr<AuthSession> authSession;
    std::shared_ptr<WorldSocket> worldSocket;
    PreparedQueryResult result;
    int64 mutetime = 0;
    int8 locale = 0;
    WorldSession* _worldSession;
    _worldSession = new WorldSession(id, "ADMIN", worldSocket, AccountTypes(0),
        2, mutetime, LOCALE_enUS, 0, false);

    Player* p = new Player(_worldSession);
    ObjectGuid playerGuid;
    playerGuid.Set(1);
    LoginQueryHolder *holder = new LoginQueryHolder(id, playerGuid);
    holder->Initialize();
    QueryResultHolderFuture _charLoginCallback = CharacterDatabase.DelayQueryHolder(holder);
    p->LoadFromDB(playerGuid, reinterpret_cast<LoginQueryHolder*>(_charLoginCallback.get()));
    p->SetSemaphoreTeleportFar(true);
    bool toNorthrend = false;
    if (p->GetMap()->GetId() == 571) {
        p->TeleportTo(608, 1808.819946, 803.929993, 44.363998, 6.282000);
    }
    else {
        toNorthrend = true;
        p->TeleportTo(571, 5680.7, 487.323, 652.418, 0.882);
    }

    _worldSession->InitializeSession();
    WorldPacket packet(MSG_MOVE_WORLDPORT_ACK, 41);
    packet.appendPackGUID(10);
    WorldPacket *packetRef = &packet;

    // Act
    _worldSession->HandleMoveWorldportAckOpcode(*packetRef);

    // Assert
    if (toNorthrend) {
        ASSERT_EQ(571, p->GetWorldLocation().GetMapId());
        ASSERT_EQ(571, p->GetMap()->GetId());
    }
    else {
        ASSERT_EQ(608, p->GetWorldLocation().GetMapId());
        ASSERT_EQ(608, p->GetMap()->GetId());
    }
}

TEST(Map, ResetDungeon)
{
    // Arrange
    int argc = 0;
    char** argv = new char*[0];
    WorldServerMoq* server = new WorldServerMoq();
    server->LaunchServer(argc, argv);
    uint32 id = 8;
    std::shared_ptr<AuthSession> authSession;
    std::shared_ptr<WorldSocket> worldSocket;
    PreparedQueryResult result;
    int64 mutetime = 0;
    int8 locale = 0;
    WorldSession* _worldSession;
    _worldSession = new WorldSession(id, "Alt", worldSocket, AccountTypes(0),
        2, mutetime, LOCALE_enUS, 0, false);

    Player* p = new Player(_worldSession);
    ObjectGuid playerGuid;
    playerGuid.Set(31);
    LoginQueryHolder *holder = new LoginQueryHolder(id, playerGuid);
    holder->Initialize();
    QueryResultHolderFuture _charLoginCallback = CharacterDatabase.DelayQueryHolder(holder);
    p->LoadFromDB(playerGuid, reinterpret_cast<LoginQueryHolder*>(_charLoginCallback.get()));
    p->SetSemaphoreTeleportFar(true);
    p->TeleportTo(34, 54.23, 0.28, -18.3424, 6.26);
    _worldSession->InitializeSession();

    WorldPacket telePacket(MSG_MOVE_TELEPORT_ACK, 41);
    telePacket.appendPackGUID(10);
    telePacket << uint32(0);
    telePacket << uint32(1);
    WorldPacket *telePacketRef = &telePacket;
    _worldSession->HandleMoveTeleportAck(*telePacketRef);

    WorldPacket portPacket(MSG_MOVE_WORLDPORT_ACK, 41);
    portPacket.appendPackGUID(10);
    WorldPacket *portPacketRef = &portPacket;
    _worldSession->HandleMoveWorldportAckOpcode(*portPacketRef);

    // Act
    Creature* c = new Creature();
    c->LoadFromDB(79035, p->GetMap(), true, false);
    c->Update(0);
    int hp = c->GetHealth();
    c->SetHealth(hp*.2);
    hp = c->GetHealth();
    int maxHp = c->GetMaxHealth();
    ASSERT_NE(maxHp, c->GetHealth());

    p->SetSemaphoreTeleportFar(true);
    p->TeleportTo(0, -8766.43, 844.892, 88.2311, 3.82961);

    WorldPacket telePacket2(MSG_MOVE_TELEPORT_ACK, 41);
    telePacket2.appendPackGUID(10);
    telePacket2 << uint32(0);
    telePacket2 << uint32(1);
    WorldPacket *telePacketRef2 = &telePacket2;
    _worldSession->HandleMoveTeleportAck(*telePacketRef2);
    _worldSession->HandleMoveWorldportAckOpcode(*portPacketRef);

    WorldPacket resetPacket(CMSG_RESET_INSTANCES, 10);
    resetPacket.appendPackGUID(797);
    WorldPacket* resetPacketRef = &resetPacket;
    _worldSession->HandleResetInstancesOpcode(*resetPacketRef);

    p->SetSemaphoreTeleportFar(true);
    p->TeleportTo(34, 54.23, 0.28, -18.3424, 6.26);

    WorldPacket telePacket3(MSG_MOVE_TELEPORT_ACK, 41);
    telePacket3.appendPackGUID(10);
    telePacket3 << uint32(0);
    telePacket3 << uint32(1);
    WorldPacket *telePacketRef3 = &telePacket3;
    _worldSession->HandleMoveTeleportAck(*telePacketRef3);
    _worldSession->HandleMoveWorldportAckOpcode(*portPacketRef);

    Creature* cTwo = new Creature();
    cTwo->LoadFromDB(79035, p->GetMap(), true, false);
    cTwo->Update(1);
    int hptwo = cTwo->GetHealth();
    int maxHptwo = cTwo->GetMaxHealth();
    ASSERT_EQ(hptwo, maxHp);

    // Assert
    ASSERT_EQ(34, p->GetMap()->GetId());
    ASSERT_TRUE(p->GetMap()->IsDungeon());
}
