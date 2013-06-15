#pragma once
#include <dctl/bit/bit.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {

template<bool Color, typename Board>
BitBoard promotion_sq(BitBoard dest_sq)
{
        return dest_sq & Board::promotion_mask[Color][0];
}

template<bool Color, typename Board>
bool is_promotion_sq(BitBoard dest_sq)
{
        return !bit::empty(promotion_sq<Color, Board>(dest_sq));
}

template<bool Color, class Position>
BitBoard promotion_squares(Position const& p)
{
        return not_occupied(p) & Position::board_type::promotion_mask[Color][0];
}

template<bool Color, class Position>
BitBoard promoting_pawns(Position const& p)
{
        return p.material().pawns(Color) & Position::board_type::promotion_mask[Color][1];
}

template<bool Color, class Position>
BitBoard non_promoting_pawns(Position const& p)
{
        return p.material().pawns(Color) & ~Position::board_type::promotion_mask[Color][1];
}

}       // namespace dctl
