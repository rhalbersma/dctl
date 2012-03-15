#pragma once
#include "Position.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<bool Color, typename Board>
BitBoard promotion_sq(BitBoard dest_sq)
{
        return dest_sq & Board::PROMOTION[Color][0];
}

template<bool Color, typename Board>
BitBoard promotion_squares(const Position<Board>& p)
{
        return not_occupied(p) & Board::PROMOTION[Color][0];
}

template<bool Color, typename Board>
BitBoard promoting_pawns(const Position<Board>& p)
{
        return p.pawns(Color) & Board::PROMOTION[Color][1];
}

template<bool Color, typename Board>
BitBoard non_promoting_pawns(const Position<Board>& p)
{
        return p.pawns(Color) & ~Board::PROMOTION[Color][1];
}

}       // namespace dctl
