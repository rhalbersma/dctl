#pragma once
#include "Position.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename Board>
int state(const Position<Board>& p)
{
        return state(p.active_color(), active_kings(p), active_pawns(p));
}

template<bool Color, typename Board>
int state(const Position<Board>& p)
{
        return state(Color, p.kings(Color), p.pawns(Color));
}

inline
int state(bool color, BitBoard kings, BitBoard pawns)
{
        return (
                ((color != 0) << 2) +
                ((kings != 0) << 1) +
                ((pawns != 0) << 0)
        );
}

}       // namespace dctl
