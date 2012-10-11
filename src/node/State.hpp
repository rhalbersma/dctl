#pragma once
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename Position>
int state(Position const& p)
{
        return state(p.active_color(), active_kings(p), active_pawns(p));
}

template<bool Color, typename Position>
int state(Position const& p)
{
        return state(Color, p.kings(Color), p.pawns(Color));
}

inline
int state(bool color, BitBoard kings, BitBoard pawns)
{
        // For purposes of move generation, a position has 8 possible states,
        // where the 3 bits of information indicate (in order of significance)
        // the side to move, and the availability of kings and pawns, respectively.
        return (
                ((color != 0) << 2) +   // most significant bit
                ((kings != 0) << 1) +
                ((pawns != 0) << 0)     // least significant bit
        );
}

}       // namespace dctl
