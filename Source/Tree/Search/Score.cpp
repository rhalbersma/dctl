#include "gtest/gtest.h"
#include "../../../../Library/Source/Tree/Search/Score.h"
#include "../../../../Library/Source/Utilities/Ply.h"

namespace tree {
namespace search {
namespace score {

TEST(Score, IsFiniteLoss)
{
        for (auto i = 0; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, is_finite(loss_value(2 * i)));
}

TEST(Score, IsNegativeLoss)
{
        for (auto i = 0; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, loss_value(2 * i) < 0);
}

TEST(Score, IsLoss)
{
        auto i = 0;
        for (; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, is_loss(loss_value(2 * i)));
        EXPECT_EQ(false, is_loss(loss_value(2 * i)));
}

TEST(Score, IsFiniteWin)
{
        for (auto i = 0; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, is_finite(win_value(2 * i + 1)));
}

TEST(Score, IsPositiveWin)
{
        for (auto i = 0; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, win_value(2 * i + 1) > 0);
}

TEST(Score, IsWin)
{
        auto i = 0;
        for (; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, is_win(win_value(2 * i + 1)));
        EXPECT_EQ(false, is_win(win_value(2 * i + 1)));
}

TEST(Score, IsWinEqualMinusLoss)
{
        for (auto i = 0; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, win_value(2 * i + 1) == -loss_value(2 * i));
}

TEST(Score, StretchLoss)
{
        auto i = 0;
        EXPECT_EQ(true, is_infinite(stretch(loss_value(2 * i))));
        for (i = 1; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, loss_value(2 * (i - 1)) == stretch(loss_value(2 * i)));
}

TEST(Score, SqueezeWin)
{
        size_t i = 0;
        for (; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, win_value(2 * (i + 1) + 1) == squeeze(win_value(2 * i + 1)));
        EXPECT_EQ(false, is_win(squeeze(win_value(2 * i + 1))));
}

}       // namespace score
}       // namespace search
}       // namespace tree
