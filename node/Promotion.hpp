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
BitBoard promoting_men(const Position<Board>& p)
{
        return p.men(Color) & Board::PROMOTION[Color][1];
}

}       // namespace dctl