#include <gtest/gtest.h>

TEST(ECSTests, BasicTest)
{
    EXPECT_EQ(2 + 2, 4);
}

TEST(ECSTests, BooleanTest)
{
    EXPECT_TRUE(true);
}

TEST(ECSTests, StringTest)
{
    std::string name = "LREngine";

    EXPECT_EQ(name, "LREngine");
}

TEST(ECSTests, UniqueTest)
{
    EXPECT_EQ(2, 2);
}