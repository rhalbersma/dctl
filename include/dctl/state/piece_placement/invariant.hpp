#pragma once
#include <dctl/board/mask.hpp>                          // squares, Promotion
#include <dctl/color.hpp>                               // black, white
#include <dctl/piece.hpp>                               // pawn, king
#include <dctl/state/piece_placement/pieces.hpp>
#include <dctl/utility/type_traits.hpp>                 // board_t
#include <cassert>

namespace dctl {
namespace detail {

template<class PiecePlacement>
constexpr auto assert_invariant(PiecePlacement const& p) noexcept
{
        assert(disjoint(p.pieces(), p.not_occupied()));

        assert(disjoint(pieces<Color::black>(p), pieces<Color::white>(p)));
        assert(disjoint(pieces<Piece::pawn >(p), pieces<Piece::king >(p)));

        assert(disjoint(pieces<Color::black, Piece::pawn>(p), pieces<Color::black, Piece::king>(p)));
        assert(disjoint(pieces<Color::white, Piece::pawn>(p), pieces<Color::white, Piece::king>(p)));
        assert(disjoint(pieces<Color::black, Piece::pawn>(p), pieces<Color::black, Piece::king>(p)));
        assert(disjoint(pieces<Color::black, Piece::pawn>(p), pieces<Color::black, Piece::king>(p)));

        assert(disjoint(pieces<Color::black, Piece::pawn>(p), board::Promotion<board_t<PiecePlacement>>::mask(Color::black)));
        assert(disjoint(pieces<Color::white, Piece::pawn>(p), board::Promotion<board_t<PiecePlacement>>::mask(Color::white)));

        assert(board::squares_v<board_type> == (p.pieces() | p.not_occupied()));

        assert(p.pieces() == (pieces<Color::black>(p) | pieces<Color::white>(p)));
        assert(p.pieces() == (pieces<Piece::pawn >(p) | pieces<Piece::king >(p)));

        assert(pieces<Color::black>(p) == (pieces<Color::black, Piece::pawn>(p) | pieces<Color::black, Piece::king>(p)));
        assert(pieces<Color::white>(p) == (pieces<Color::white, Piece::pawn>(p) | pieces<Color::white, Piece::king>(p)));
        assert(pieces<Piece::pawn >(p) == (pieces<Color::black, Piece::pawn>(p) | pieces<Color::white, Piece::pawn>(p)));
        assert(pieces<Piece::king >(p) == (pieces<Color::black, Piece::king>(p) | pieces<Color::white, Piece::king>(p)));
}

}       // namespace detail
}       // namespace dctl
