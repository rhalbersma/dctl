#pragma once
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<bool Color, typename Board>
bool is_promotion_sq(BitBoard dest_sq)
{
        return !bit::is_zero(promotion_sq<Color, Board>(dest_sq));
}

template<bool Color, typename Board>
BitBoard promotion_sq(BitBoard dest_sq)
{
        return dest_sq & Board::PROMOTION[Color][0];
}

template<bool Color, typename Rules, typename Board, template<typename, typename> class Position>
BitBoard promotion_squares(const Position<Rules, Board>& p)
{
        return not_occupied(p) & Board::PROMOTION[Color][0];
}

template<bool Color, typename Rules, typename Board, template<typename, typename> class Position>
BitBoard promoting_pawns(const Position<Rules, Board>& p)
{
        return p.pawns(Color) & Board::PROMOTION[Color][1];
}

template<bool Color, typename Rules, typename Board, template<typename, typename> class Position>
BitBoard non_promoting_pawns(const Position<Rules, Board>& p)
{
        return p.pawns(Color) & ~Board::PROMOTION[Color][1];
}

}       // namespace dctl
