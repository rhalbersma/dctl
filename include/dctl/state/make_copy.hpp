#pragma once

namespace dctl {

template<class State, class Action>
auto make_copy(State const& p, Action const& m)
{
        auto q = p;
        q.attach(p);
        q.make(m);
        return q;
}

}       // namespace dctl
