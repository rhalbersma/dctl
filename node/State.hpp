#pragma once
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename> class Position;

template<typename Board>
int state(const Position<Board>& p)
{
        return state(p.active_color(), active_kings(p), active_men(p));
}

template<bool Color, typename Board>
int state(const Position<Board>& p)
{
        return state(Color, p.kings(Color), p.men(Color));
}

inline int state(bool color, BitBoard kings, BitBoard men)
{
        return (
                ((color != 0) << 2) +
                ((kings != 0) << 1) +
                ((  men != 0) << 0)
        );
}

}       // namespace dctl
