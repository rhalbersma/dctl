#include "gtest/gtest.h"
#include "../../../DCTL/src/search/Score.h"
#include "../../../DCTL/src/utils/Ply.h"

namespace dctl {
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
        for (auto i = 0; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, is_loss(loss_value(2 * i)));
        EXPECT_EQ(false, is_loss(loss_value(MAX_GRAFTED_PLY)));
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
        for (auto i = 0; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, is_win(win_value(2 * i + 1)));
        EXPECT_EQ(false, is_win(win_value(MAX_GRAFTED_PLY + 1)));
}

TEST(Score, IsWinEqualMinusLoss)
{
        for (auto i = 0; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, win_value(2 * i + 1) == -loss_value(2 * i));
}

TEST(Score, StretchLoss)
{        
        EXPECT_EQ(true, is_infinite(stretch(loss_value(0))));
        for (auto i = 1; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, loss_value(2 * (i - 1)) == stretch(loss_value(2 * i)));
}

TEST(Score, SqueezeWin)
{        
        for (auto i = 0; i < MAX_GRAFTED_PLY / 2; ++i)
                EXPECT_EQ(true, win_value(2 * (i + 1) + 1) == squeeze(win_value(2 * i + 1)));
        EXPECT_EQ(false, is_win(squeeze(win_value(MAX_GRAFTED_PLY + 1))));
}

}       // namespace score
}       // namespace search
}       // namespace dctl
