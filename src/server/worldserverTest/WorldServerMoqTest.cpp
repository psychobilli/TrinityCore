#include <gmock/gmock.h>
using ::testing::Eq;
#include <gtest/gtest.h>
using ::testing::Test;

#include "WorldServerMoq.h"
#include "WorldSocketMgr.h"
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
