#pragma once

namespace dctl {

template<class State, class Action>
auto result(State const& s, Action const& a) noexcept
{
        State nrv{s}; nrv.make(a); return nrv;
}

}       // namespace dctl
