#pragma once

//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cassert>                              // assert
#include <cstdint>                              // uint64_t
#include <tuple>                                // tie
#include <type_traits>                          // is_same

namespace dctl::algo {

template<class State, class Action>
constexpr auto result(State const& s, Action const& a) noexcept
{
        auto nrv{s}; nrv.make(a); return nrv;
}

template<class State, class Action>
class node
{
        constexpr auto is_root() const noexcept
        {
                return
                        !m_parent &&
                        !m_action
                ;
        }

        constexpr auto is_child() const noexcept
        {
                return
                        m_parent &&
                        m_action &&
                        m_state == result(m_parent->m_state, *m_action)
                ;
        }

        State m_state;
        node const* m_parent = nullptr;
        Action const* m_action = nullptr;
        int m_cost {};
        uint64_t m_hash{};
public:
        using state_type = State;
        using action_type = Action;

        explicit constexpr node(State const& s) noexcept
        :       m_state{s}
        ,       m_hash{}
        {
                assert(is_root());
        }

        constexpr node(node const& n, Action const& a) noexcept
        :       m_state{result(n.m_state, a)}
        ,       m_parent{&n}
        ,       m_action{&a}
        ,       m_cost{n.m_cost + 1}
        ,       m_hash{}
        {
                assert(is_child());
        }

        constexpr auto const& state() const noexcept { return m_state; }
        auto cost() const { return m_cost; }
        auto hash() const { return 0ull; }// m_hash; }

        constexpr auto tied() const
        {
                return std::tie(m_state);
        }
};

template<class Node, class State>
constexpr auto root(State const& s) noexcept
{
        return Node{s};
}

template<class Node, class Action>
constexpr auto child(Node const& n, Action const& a) noexcept
{
        return Node{n, a};
}

template<class State, class Action>
constexpr auto operator==(node<State, Action> const& lhs, node<State, Action> const& rhs) noexcept
{
        return lhs.tied() == rhs.tied();
}

template<class State, class Action>
constexpr auto operator< (node<State, Action> const& lhs, node<State, Action> const& rhs) noexcept
{
        return lhs.tied() < rhs.tied();
}

template<class State, class Action>
constexpr auto operator!=(node<State, Action> const& lhs, node<State, Action> const& rhs) noexcept
{
        return !(lhs == rhs);
}

template<class State, class Action>
constexpr auto operator> (node<State, Action> const& lhs, node<State, Action> const& rhs) noexcept
{
        return rhs < lhs;
}

template<class State, class Action>
constexpr auto operator>=(node<State, Action> const& lhs, node<State, Action> const& rhs) noexcept
{
        return !(lhs < rhs);
}

template<class State, class Action>
constexpr auto operator<=(node<State, Action> const& lhs, node<State, Action> const& rhs) noexcept
{
        return !(rhs < lhs);
}

/*
template<class State, class Action>
class node
{
        State state_;
        node const* parent_;
        std::optional<Action> action_;
public:
        constexpr node(State const& s, node const* n = nullptr, std::optional<Action> const& a = std::nullopt_t) noexcept
        :
                state_{s},
                parent_{n},
                action_{a}
        {}

        template<class Problem>
        auto expand(Problem const& problem)
        {
                auto const actions = problem.actions(state_);
                std::vector<node> children(actions.size());
                std::transform(actions.cbegin(), actions.cend(), children.begin(), [](auto const& action) {
                        return child_node(problem, action);
                });
                return children;
        }

        template<class Problem, class Action>
        auto child_node(Problem const& problem, Action const& action) const
        {
                return node{problem.result(state_, action), this, action};
        }

        auto solution() const
        {
                auto const p = path();
                std::vector<Action> s(p.size() - 1);
                std::transform(std::next(p.cbegin()), p.cend(), s.begin(), [](auto const& n) {
                        assert(n.action_);
                        return *n.action_;
                });
                return s;
        }

        auto path() const
        {
                std::vector<node> p;
                auto const* n = this;
                do {
                        p.push_back(*n);
                        n = n->parent_;
                } while (n != nullptr);
                std::reverse(p.begin(), p.end());
                return p;
        }
};

template<class T, class InternalHash = acme::fnv1a>
class wrap
{
        T m_value;
        typename InternalHash::result_type m_hash;
public:
        explicit wrap(T const& u) noexcept
        :       m_value{u}
        ,       m_hash{xstd::uhash<InternalHash>{}(m_value)}
        {}

};

std::unordered_set<wrap, acme::identity_hash>;
*/
}       // namespace dctl::algo
