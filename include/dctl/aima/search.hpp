#pragma once
#include <dctl/aima/problem.hpp>
#include <dctl/aima/node.hpp>
#include <experimental/optional>
#include <queue>
#include <stack>

namespace dctl {
namespace aima {

template<class Problem>
using node_t = node<state_t<Problem>, action_t<Problem>>;

template<class Problem, class Frontier>
auto tree_search(Problem const& problem, Frontier& frontier)
        -> std::optional<node_t<Problem>>
{
        assert(frontier.empty());
        frontier.push(problem.initial());
        while (!frontier.empty()) {
                auto const node = first_out(frontier);
                frontier.pop();
                if (problem.goal_test(node))
                        return node;
                auto const children = node.expand(problem);
                for (auto const& child : children)
                        frontier.push(child);
        }
        return std::nullopt_t;
}

template<class T, class Container>
auto& first_out(std::stack<T, Container>& s)
{
        return s.top();
}

template<class T, class Container>
auto const& first_out(std::stack<T, Container> const& s)
{
        return s.top();
}

template<class Problem>
auto depth_first_tree_search(Problem const& problem, std::stack<node_t<Problem>>& frontier)
{
        return tree_search(problem, frontier);
}

template<class T, class Container>
auto& first_out(std::queue<T, Container>& q)
{
        return q.front();
}

template<class T, class Container>
auto const& first_out(std::queue<T, Container> const& q)
{
        return q.front();
}

template<class Problem>
auto best_first_tree_search(Problem const& problem, std::queue<node_t<Problem>>& frontier)
{
        return tree_search(problem, frontier);
}

}       // namespace aima
}       // namespace dctl
