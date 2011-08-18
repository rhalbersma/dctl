#include "Position.h"

namespace dctl {

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

}       // namespace dctl
