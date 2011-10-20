#include <boost/test/unit_test.hpp> 
#include "../../src/search/Score.hpp"
#include "../../src/utility/Ply.hpp"

namespace dctl {
namespace search {

BOOST_AUTO_TEST_SUITE(TestScore)

BOOST_AUTO_TEST_CASE(IsFiniteLoss)
{
        for (auto i = 0; i <= MAX_MATE_MOVES; ++i)
                BOOST_CHECK(is_finite(loss_value(i)));
}

BOOST_AUTO_TEST_CASE(IsNegativeLoss)
{
        for (auto i = 0; i <= MAX_MATE_MOVES; ++i)
                BOOST_CHECK_LT(loss_value(2 * i), 0);
}

BOOST_AUTO_TEST_CASE(IsLoss)
{
        for (auto i = 0; i <= MAX_MATE_MOVES; ++i)
                BOOST_CHECK(is_loss(loss_value(i)));
        BOOST_CHECK(!is_loss(loss_value(MAX_MATE_MOVES + 1)));
}

BOOST_AUTO_TEST_CASE(IsFiniteWin)
{
        for (auto i = 0; i <= MAX_MATE_MOVES; ++i)
                BOOST_CHECK(is_finite(win_value(i)));
}

BOOST_AUTO_TEST_CASE(IsPositiveWin)
{
        for (auto i = 0; i <= MAX_MATE_MOVES; ++i)
                BOOST_CHECK_GT(win_value(i), 0);
}

BOOST_AUTO_TEST_CASE(IsWin)
{        
        for (auto i = 0; i <= MAX_MATE_MOVES; ++i)
                BOOST_CHECK(is_win(win_value(i)));
        BOOST_CHECK(!is_win(win_value(MAX_MATE_MOVES + 1)));
}

BOOST_AUTO_TEST_CASE(IsWinEqualMinusLoss)
{
        for (auto i = 0; i <= MAX_MATE_MOVES; ++i)
                BOOST_CHECK_EQUAL(win_value(i), -loss_value(i));
}

BOOST_AUTO_TEST_CASE(Stretch)
{        
        BOOST_CHECK(is_infinite(-stretch(loss_value(0))));
        BOOST_CHECK(is_infinite(-stretch(win_value(0))));
        for (auto i = 1; i <= MAX_MATE_MOVES; ++i) {
                BOOST_CHECK_EQUAL(win_value(i - 1), -stretch(loss_value(i)));
                BOOST_CHECK_EQUAL(loss_value(i - 1), -stretch(win_value(i)));
        }
}

BOOST_AUTO_TEST_CASE(Squeeze)
{        
        for (auto i = 0; i < MAX_MATE_MOVES; ++i) {
                BOOST_CHECK_EQUAL(win_value(i + 1), -squeeze(loss_value(i)));
                BOOST_CHECK_EQUAL(loss_value(i + 1), -squeeze(win_value(i)));
        }
        BOOST_CHECK(!is_win(-squeeze(loss_value(MAX_MATE_MOVES))));
        BOOST_CHECK(!is_loss(-squeeze(win_value(MAX_MATE_MOVES))));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace search
}       // namespace dctl
