#pragma once

namespace dctl {

template<class Position, class Move>
auto make_copy(Position const& p, Move const& m)
{
        auto q = p;
        q.attach(p);
        q.make(m);
        return q;
}

}       // namespace dctl
