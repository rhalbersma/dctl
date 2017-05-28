#include <dctl/core/color_piece.hpp>         // color, black, white, opposite, piece, pawns, kings
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE
#include <type_traits>                  // is_same

BOOST_AUTO_TEST_SUITE(ColorPiece)

using namespace dctl;

BOOST_AUTO_TEST_CASE(ColorsAreDifferent)
{
        static_assert(color::black != color::white);
        static_assert(color::white != color::black);

        static_assert(!std::is_same<black_, white_>{});
        static_assert(!std::is_same<white_, black_>{});

        static_assert(black_c != white_c);
        static_assert(white_c != black_c);
}

BOOST_AUTO_TEST_CASE(ColorsAreNegativelyRelated)
{
        static_assert((!color::black) == color::white);
        static_assert((!color::white) == color::black);

        static_assert(std::is_same<opposite<black_>, white_>{});
        static_assert(std::is_same<opposite<white_>, black_>{});

        static_assert((!black_c) == white_c);
        static_assert((!white_c) == black_c);
}

BOOST_AUTO_TEST_CASE(PiecesAreDifferent)
{
        static_assert(piece::pawns != piece::kings);
        static_assert(piece::kings != piece::pawns);

        static_assert(!std::is_same<pawns_, kings_>{});
        static_assert(!std::is_same<kings_, pawns_>{});

        static_assert(pawns_c != kings_c);
        static_assert(kings_c != pawns_c);
}

BOOST_AUTO_TEST_SUITE_END()
