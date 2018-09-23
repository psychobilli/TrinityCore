#include <gmock/gmock.h>
using ::testing::Eq;
#include <gtest/gtest.h>
using ::testing::Test;

#include "main_moq.h"
char *defaults[] = { "h","e","l","p" };

TEST(WorldServer, Main)
{
    int argc = 0;
    char **argv = new char*[0];
    int result = -1;
    main_moq* main = new main_moq();
    result = main->main(argc, argv);
    result = main->Shutdown();

    ASSERT_EQ(0, result);
}

TEST(WorldServer, GetWorldSocket)
{
    int argc = 0;
    char **argv = new char*[0];
    main_moq* main = new main_moq();
    main->main(argc, argv);
    std::shared_ptr<WorldSocket> socket = main->GetWorldSocket();

    ASSERT_TRUE(socket);
}
