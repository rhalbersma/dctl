//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/piece.hpp>    // piece, pawn, king, pawn_, king_, pawn_c, king_c
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE
#include <type_traits>                  // is_same

BOOST_AUTO_TEST_SUITE(ColorPiece)

using namespace dctl::core;

BOOST_AUTO_TEST_CASE(PiecesAreDifferent)
{
        static_assert(piece::pawn != piece::king);
        static_assert(piece::king != piece::pawn);

        static_assert(!std::is_same_v<pawn_, king_>);
        static_assert(!std::is_same_v<king_, pawn_>);

        static_assert(pawn_c != king_c);
        static_assert(king_c != pawn_c);
}

BOOST_AUTO_TEST_SUITE_END()
