#include <gmock/gmock.h>
using ::testing::Eq;
#include <gtest/gtest.h>
using ::testing::Test;

#include "WorldServerMoq.h"
#include <boost/asio/ip/tcp.hpp>
#include <functional>

TEST(WorldSession, CMSG_AUTH_SESSION)
{
    int argc = 0;
    char** argv = new char*[0];
    WorldServerMoq* server = new WorldServerMoq();
    server->LaunchServer(argc, argv);

    std::function<std::pair<tcp::socket*, uint32>()> _socketFactory;
    tcp::socket* socket;
    uint32 threadIndex;
    std::pair<tcp::socket*, uint32> defaultFactory(std::make_pair(socket, 0));
//    AsyncAcceptor *acceptor = main_moq::GetAcceptor();
    //_socketFactory(std::bind(&defaultFactory, *acceptor));
    std::tie(socket, threadIndex) = _socketFactory();
    //WorldSocket worldSocket();
    /*WorldSocket worldSocket();
    sWorldSocketMgr.OnSocketOpen(worldSocket, 0);*/
    //tcp::socket *socket = new tcp::socket();
    //WorldSocket *worldSocket = new WorldSocket(&socket);
    ASSERT_EQ(0, 0);
}
