#include "gtest/gtest.h"
#include "../../../../Library/Source/Tree/Search/Value.h"
#include "../../../../Library/Source/Utilities/Ply.h"

namespace tree {
namespace search {
namespace value {

TEST(Value, IsFiniteLoss)
{
        for (size_t i = 0; i < GRAFTED_PLY_MAX / 2; ++i)
                EXPECT_EQ(true, is_finite(loss(2 * i)));
}

TEST(Value, IsNegativeLoss)
{
        for (size_t i = 0; i < GRAFTED_PLY_MAX / 2; ++i)
                EXPECT_EQ(true, loss(2 * i) < 0);
}

TEST(Value, IsLoss)
{
        size_t i = 0;
        for (; i < GRAFTED_PLY_MAX / 2; ++i)
                EXPECT_EQ(true, is_loss(loss(2 * i)));
        EXPECT_EQ(false, is_loss(loss(2 * i)));
}

TEST(Value, IsFiniteWin)
{
        for (size_t i = 0; i < GRAFTED_PLY_MAX / 2; ++i)
                EXPECT_EQ(true, is_finite(win(2 * i + 1)));
}

TEST(Value, IsPositiveWin)
{
        for (size_t i = 0; i < GRAFTED_PLY_MAX / 2; ++i)
                EXPECT_EQ(true, win(2 * i + 1) > 0);
}

TEST(Value, IsWin)
{
        size_t i = 0;
        for (; i < GRAFTED_PLY_MAX / 2; ++i)
                EXPECT_EQ(true, is_win(win(2 * i + 1)));
        EXPECT_EQ(false, is_win(win(2 * i + 1)));
}

TEST(Value, IsWinEqualMinusLoss)
{
        for (size_t i = 0; i < GRAFTED_PLY_MAX / 2; ++i)
                EXPECT_EQ(true, win(2 * i + 1) == -loss(2 * i));
}

TEST(Value, StretchLoss)
{
        size_t i = 0;
        EXPECT_EQ(true, is_infinite(stretch(loss(2 * i))));
        for (i = 1; i < GRAFTED_PLY_MAX / 2; ++i)
                EXPECT_EQ(true, loss(2 * (i - 1)) == stretch(loss(2 * i)));
}

TEST(Value, SqueezeWin)
{
        size_t i = 0;
        for (; i < GRAFTED_PLY_MAX / 2; ++i)
                EXPECT_EQ(true, win(2 * (i + 1) + 1) == squeeze(win(2 * i + 1)));
        EXPECT_EQ(false, is_win(squeeze(win(2 * i + 1))));
}

}       // namespace value
}       // namespace search
}       // namespace tree
