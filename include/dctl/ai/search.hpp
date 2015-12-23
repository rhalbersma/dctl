#pragma once
#include <experimental/optional>

namespace dctl {

template<class Problem, class Frontier>
auto tree_search(Problem const& problem, Frontier const& frontier)
        -> std::optional<Node>
{
        assert(frontier.empty());
        frontier.push(problem.initial());
        while (!frontier.empty()) {
                auto const node = frontier.pop();
                if (problem.goal_test(node))
                        return node;
                node.expand(problem, frontier);
        }
        return std::nullopt_t;
}

template<class Problem>
auto depth_first_tree_search(Problem const& problem)
{
        return tree_search(problem, std::stack<node_t<Problem>>{});
}

template<class Problem>
auto best_first_tree_search(Problem const& problem)
{
        return tree_search(problem, std::queue<node_t<Problem>>{});
}

template<class Problem, class Frontier, class Explored>
auto graph_search(Problem const& problem, Frontier frontier, Explored explored)
        -> std::optional<Node>
{
        assert(frontier.empty() && explored.empty());
        frontier.push(problem.initial());
        while (!frontier.empty()) {
                auto const node = frontier.pop();
                if (problem.goal_test(node))
                        return node;
                explored.push(node);
                node.expand(problem, frontier, explored);
        }
        return std::nullopt_t;
}


}
