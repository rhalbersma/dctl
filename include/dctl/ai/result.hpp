#pragma once

namespace dctl {

template<class State, class Action>
auto result(State const& s, Action const& a) noexcept
{
        State nrv{s}; nrv.make(a); return nrv;
}

template<class State, class Action>
auto origin(State const& s, Action const& a) noexcept
{
        State nrv{s}; nrv.undo(a); return nrv;
}

}       // namespace dctl
