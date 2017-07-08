//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/algo/search/ply.hpp>
#include <dctl/algo/search/score.hpp>
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK, BOOST_CHECK_EQUAL, BOOST_CHECK_LT, BOOST_AUTO_TEST_SUITE_END

using namespace dctl::algo::search;

BOOST_AUTO_TEST_SUITE(SearchScore)

BOOST_AUTO_TEST_CASE(IsLoss)
{
        // minus infinity is not a loss
        BOOST_CHECK(!is_loss(-infinity()));

        // losses start one above minus infinity
        BOOST_CHECK_EQUAL(-infinity() + 1, loss_min());

        // scores in the interval [loss_min, loss_max) are losses
        for (auto v = loss_min(); v < loss_max(); ++v)
                BOOST_CHECK(is_loss(v));

        // all other scores in the [loss_max, +INF] interval are not losses
        for (auto v = loss_max(); v <= infinity(); ++v)
                BOOST_CHECK(!is_loss(v));
}

BOOST_AUTO_TEST_CASE(FiniteLoss)
{
        // scores in the interval [loss_min, loss_max) are finite
        for (auto v = loss_min(); v < loss_max(); ++v)
                BOOST_CHECK(is_finite(v));
}

BOOST_AUTO_TEST_CASE(NegativeLoss)
{
        // scores in the interval [loss_min, loss_max) are negative
        for (auto v = loss_min(); v < loss_max(); ++v)
                BOOST_CHECK_LT(v, 0);
}

BOOST_AUTO_TEST_CASE(IsWin)
{
        // infinity is not a win
        BOOST_CHECK(!is_win(infinity()));

        // wins start at one below infinity
        BOOST_CHECK_EQUAL(infinity() - 1, win_min());

        // scores in the interval (win_max, win_min] are wins
        for (auto v = win_min(); v > win_max(); --v)
                BOOST_CHECK(is_win(v));

        // all other scores in the interval [-INF, win_max] are not wins
        for (auto v = -infinity(); v <= win_max(); ++v)
                BOOST_CHECK(!is_win(v));
}

BOOST_AUTO_TEST_CASE(FiniteWin)
{
        // scores in the interval (win_max, win_min] are finite
        for (auto v = win_min(); v > win_max(); --v)
                BOOST_CHECK(is_finite(v));
}

BOOST_AUTO_TEST_CASE(PositiveWin)
{
        // scores in the interval (win_max, win_min] are positive
        for (auto v = win_min(); v > win_max(); --v)
                BOOST_CHECK_GT(v, 0);
}

BOOST_AUTO_TEST_CASE(WinEqualMinusLoss)
{
        for (auto i = 0; i < MAX_MATE_MOVES; ++i)
                BOOST_CHECK_EQUAL(win_value(i), -loss_value(i));
}

BOOST_AUTO_TEST_CASE(MinMaxDistance)
{
        BOOST_CHECK_EQUAL(loss_min(), loss_value(0));
        BOOST_CHECK_EQUAL(loss_max(), loss_value(MAX_MATE_MOVES));
        BOOST_CHECK_EQUAL( win_min(),  win_value(0));
        BOOST_CHECK_EQUAL( win_max(),  win_value(MAX_MATE_MOVES));
}

BOOST_AUTO_TEST_CASE(PlyValueInverse)
{
        for (auto i = 0; i < MAX_MATE_MOVES; ++i) {
                BOOST_CHECK_EQUAL(i, loss_ply(loss_value(i)));
                BOOST_CHECK_EQUAL(i,  win_ply( win_value(i)));
        }
}

BOOST_AUTO_TEST_CASE(Stretch)
{
        // loss and win values are "stretched" one step towards the edges of the [-INF, +INF] interval
        for (auto i = 1; i < MAX_MATE_MOVES; ++i) {
                BOOST_CHECK_EQUAL(loss_value(i - 1), stretch(loss_value(i)));
                BOOST_CHECK_EQUAL( win_value(i - 1), stretch( win_value(i)));
        }

        // heuristic scores are invariant
        for (auto v = loss_max(); v <= win_max(); ++v)
                BOOST_CHECK_EQUAL(v, stretch(v));

        // infinite scores are invariant
        BOOST_CHECK_EQUAL(-infinity(), stretch(loss_value(0)));
        BOOST_CHECK_EQUAL( infinity(), stretch( win_value(0)));
}

BOOST_AUTO_TEST_CASE(Squeeze)
{
        // loss and win values are "squeezed" one step towards the center of the [-INF, +INF] interval
        for (auto i = 0; i < MAX_MATE_MOVES; ++i) {
                BOOST_CHECK_EQUAL(loss_value(i + 1), squeeze(loss_value(i)));
                BOOST_CHECK_EQUAL( win_value(i + 1), squeeze( win_value(i)));
        }

        // heuristic scores are invariant
        for (auto v = loss_max(); v <= win_max(); ++v)
                BOOST_CHECK_EQUAL(v, squeeze(v));

        // infinite scores are invariant
        BOOST_CHECK_EQUAL(-infinity(), squeeze(-infinity()));
        BOOST_CHECK_EQUAL( infinity(), squeeze( infinity()));
}

BOOST_AUTO_TEST_SUITE_END()
