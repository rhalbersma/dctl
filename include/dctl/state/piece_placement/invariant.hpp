#pragma once
#include <dctl/board/mask.hpp>  // squares, Promotion
#include <dctl/color.hpp>       // black, white
#include <dctl/piece.hpp>       // pawn, king
#include <dctl/utility/type_traits.hpp> // board_type

namespace dctl {
namespace detail {

template<class PiecePlacement>
constexpr auto invariant(PiecePlacement const& p) noexcept
{
        using board_type = board_type_t<PiecePlacement>;

        return
                 (p.pieces(Color::black, Piece::pawn) ^ p.pieces(Color::black, Piece::king)) == p.pieces(Color::black) &&
                 (p.pieces(Color::white, Piece::pawn) ^ p.pieces(Color::white, Piece::king)) == p.pieces(Color::white) &&
                 (p.pieces(Color::black, Piece::pawn) ^ p.pieces(Color::white, Piece::pawn)) == p.pieces(Piece::pawn ) &&
                 (p.pieces(Color::black, Piece::king) ^ p.pieces(Color::white, Piece::king)) == p.pieces(Piece::king ) &&
                 (p.pieces(Color::black) ^ p.pieces(Color::white)) == p.pieces() &&
                 (p.pieces(Piece::pawn ) ^ p.pieces(Piece::king )) == p.pieces() &&
                 (p.pieces() ^ p.not_occupied()) == board::squares_v<board_type> &&
                 disjoint(p.pieces(Color::black), p.pieces(Color::white)) &&
                 disjoint(p.pieces(Piece::pawn ), p.pieces(Piece::king )) &&
                 disjoint(p.pieces(), p.not_occupied()) &&
                 disjoint(p.pieces(Color::black, Piece::pawn), board::Promotion<board_type>::mask(Color::black)) &&
                 disjoint(p.pieces(Color::white, Piece::pawn), board::Promotion<board_type>::mask(Color::white))
        ;
}

}       // namespace detail
}       // namespace dctl
