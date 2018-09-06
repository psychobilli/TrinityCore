#include <gmock/gmock.h>
  using ::testing::Eq;
#include <gtest/gtest.h>
  using ::testing::Test;

class worldserverTest : public Test
{
protected:
    worldserverTest(){}
    ~worldserverTest(){}

    virtual void SetUp(){}
    virtual void TearDown(){}
};