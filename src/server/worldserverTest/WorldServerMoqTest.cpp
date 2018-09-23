#include <gmock/gmock.h>
using ::testing::Eq;
#include <gtest/gtest.h>
using ::testing::Test;

#include "WorldServerMoq.h"
char *defaults[] = { "h","e","l","p" };

TEST(WorldServerMoq, LaunchServer)
{
    int argc = 0;
    char **argv = new char*[0];
    int result = -1;
    WorldServerMoq* server = new WorldServerMoq();
    result = server->LaunchServer(argc, argv);
    result = server->Shutdown();

    ASSERT_EQ(0, result);
}

TEST(WorldServerMoq, GetWorldSocket)
{
    int argc = 0;
    char **argv = new char*[0];
    WorldServerMoq* server = new WorldServerMoq();
    server->LaunchServer(argc, argv);
    std::shared_ptr<WorldSocket> socket = server->GetWorldSocket();

    ASSERT_TRUE(socket);
}
