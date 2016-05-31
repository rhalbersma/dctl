#pragma once
#include <cassert>                      // assert
#include <algorithm>                    // reverse, transform
#include <vector>                       // vector
#include <experimental/optional>        // nullopt_t, optional

namespace dctl {
namespace aima {

template<class State, class Action>
constexpr auto result(State const& s, Action const& a) noexcept
{
        State nrv{s}; nrv.make(a); return nrv;
}

template<class State, class Action>
struct node
{
        State state;
        node const* parent = nullptr;
        Action const* action = nullptr;

        constexpr auto is_root() const noexcept
        {
                return !parent && !action;
        }

        constexpr auto is_child() const noexcept
        {
                return parent && action && result(parent->state, *action) == state;
        }

public:
        constexpr node(State const& s) noexcept
        :
                state{s}
        {
                assert(is_root());
        }

        constexpr node(node const& n, Action const& a) noexcept
        :
                state{result(n.state, a)},
                parent{&n},
                action{&a}
        {
                assert(is_child());
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
*/
}       // namespace aima
}       // namespace dctl
