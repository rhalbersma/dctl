#pragma once
#include <vector>

namespace dctl {

template<class State>
class Problem
{
        State initial;

        using action_type = action_t<Actions>;
        std::vector<std::vector<action_type>> actions;
public :
        Problem(State const& s) : initial{s} {}


};

template<class State>
class Node
{
        State state;
        State const* parent;
        int depth;

public:
        Node(State const&, State const*);


};

template<class Node, class Action>
auto child_node(Node const& n, Action const& a)
{
        return Node{result(n.state, a), &n.state};
}

}       // namespace dctl
