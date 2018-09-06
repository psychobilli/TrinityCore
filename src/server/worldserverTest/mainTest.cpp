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
    int result = main_moq::main(argc, argv);

    ASSERT_EQ(0, result);
}
