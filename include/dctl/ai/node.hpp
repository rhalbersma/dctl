#pragma once
#include <cassert>      // assert

namespace dctl {

template<class State, class Action>
constexpr auto result(State const& s, Action const& a) noexcept
{
        State nrv{s}; nrv.make(a); return nrv;
}

template<class State, class Action>
class Node
{
        State state;
        Node const* parent = nullptr;
        Action const* action = nullptr;

        constexpr auto invariant() const noexcept
        {
                assert(!(parent || action) || result(parent->state, action) == state);
        }

        constexpr Node(Node const& n, Action const& a) noexcept
        :
                state{result(n.state, a)},
                parent{&n},
                action{&a}
        {
                assert(invariant());
        }

public:
        constexpr Node(State const& s) noexcept
        :
                state{s}
        {
                assert(invariant());
        }

        friend constexpr auto child(Node const& n, Action const& a) noexcept
        {
                return Node{n, a};
        }
};

}       // namespace dctl
