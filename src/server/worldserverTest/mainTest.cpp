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
    result = main_moq::main(argc, argv);
    result = main_moq::Shutdown();

    ASSERT_EQ(0, result);
}

TEST(WorldServer, GetWorldSocket)
{
    int argc = 0;
    char **argv = new char*[0];
    main_moq::main(argc, argv);
    std::shared_ptr<WorldSocket> socket = main_moq::GetWorldSocket();

    ASSERT_TRUE(socket);
}
