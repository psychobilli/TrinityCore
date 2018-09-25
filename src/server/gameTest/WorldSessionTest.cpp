#include <gmock/gmock.h>
using ::testing::Eq;
#include <gtest/gtest.h>
using ::testing::Test;

#include "Player.h"
#include "CharacterHandler.cpp"
#include "WorldServerMoq.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include <boost/asio/ip/tcp.hpp>
#include <functional>

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

TEST(WorldSession, CMSG_AUTH_SESSION)
{
    int argc = 0;
    char** argv = new char*[0];
    WorldServerMoq* server = new WorldServerMoq();
    server->LaunchServer(argc, argv);

    //tcp::socket&& socket();
    //WorldSocket *worldSocket = new WorldSocket(socket);
    std::shared_ptr<WorldSocket> worldSocket;
    MessageBuffer mb = worldSocket->GetReadBuffer();

    ASSERT_EQ(0, 0);
}

TEST(WorldSession, HandleMoveTeleportAck)
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

    Player* p = new Player(_worldSession);
    ObjectGuid playerGuid;
    playerGuid.Set(1);
    LoginQueryHolder *holder = new LoginQueryHolder(id, playerGuid);
    holder->Initialize();
    QueryResultHolderFuture _charLoginCallback = CharacterDatabase.DelayQueryHolder(holder);
    p->LoadFromDB(playerGuid, reinterpret_cast<LoginQueryHolder*>(_charLoginCallback.get()));

    _worldSession->InitializeSession();
    WorldPacket packet(MSG_MOVE_TELEPORT_ACK, 41);
    packet.appendPackGUID(10);
    packet << uint32(0);
    packet << uint32(1);
    WorldPacket *packetRef = &packet;

    _worldSession->HandleMoveTeleportAck(*packetRef);
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
    packet << uint32(0);
    packet << uint32(1);
    WorldPacket *packetRef = &packet;

    // Act
    _worldSession->HandleMoveWorldportAckOpcode(*packetRef);

    // Assert
    if (toNorthrend) {
        ASSERT_EQ(571, p->GetWorldLocation().GetMapId());
    }
    else {
        ASSERT_EQ(608, p->GetWorldLocation().GetMapId());
    }
}
