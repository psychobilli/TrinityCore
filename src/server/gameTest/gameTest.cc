#include <gmock/gmock.h>
  using ::testing::Eq;
#include <gtest/gtest.h>
  using ::testing::Test;


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

    TEST_F(gameTest, doesAThing)
    {
        EXPECT_THAT(0, Eq(0));
    }

} // namespace testing
} // namespace game
