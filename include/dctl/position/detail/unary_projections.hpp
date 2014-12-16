#pragma once
#include <dctl/color.hpp>
#include <dctl/piece.hpp>
#include <dctl/position/mrp_king/mrp_king.hpp>  // is_max
#include <type_traits>                          // false_type, true_type

namespace dctl {
namespace detail {

// unrestricted consecutive moves with the same king
template<class Position>
auto moveable_kings(Position const& p, Color c, std::false_type)
{
        return p.pieces(c, Piece::king);
}

// restricted consecutive moves with the same king
template<class Position>
auto moveable_kings(Position const& p, Color c, std::true_type)
{
        auto kings = p.pieces(c, Piece::king);
        if (p.pieces(c, Piece::king).any() && p.pieces(c, Piece::pawn).any() && p.mrp_king(c).is_max())
                kings.reset(p.mrp_king(c).square());
        return kings;
}

// pawns that cannot capture kings
template<Color ToMove, class Position>
auto targets(Position const& p, std::false_type)
{
        return p.pieces(!ToMove, Piece::pawn);
}

// pawns that can capture kings
template<Color ToMove, class Position>
auto targets(Position const& p, std::true_type)
{
        return p.pieces(!ToMove);
}

}       // namespace detail
}       // namespace dctl
