#pragma once

namespace dctl {

template<class State, class Action>
class Node
{
        State state_;
        Node const* parent_;
        Action action_;
public:
        Node(State const& s, Node const* p, Action const& a)
        :
                state_{s}, parent_{p}, action_{a}
        {}

        auto state() const
        {
                return state_;
        }

        auto parent() const
        {
                return parent_;
        }

        auto action() const
        {
                return action_;
        }
};

template<class Node, class Action>
auto child_node(Node const& parent, Action const& action)
{
        return Node{result(parent.state(), action), parent, action};
}

}       // namespace dctl
