#pragma once
#include <dctl/tree/link/i_single.hpp>

namespace dctl {
namespace tree {

template<typename T>
struct Node
:
        // Curiously Recurring Template Pattern (CRTP)
        public link::ISingle<Node, T>
{
public:
        // structors

        Node(T const& v)
        :
                next_(nullptr),
                value_(v)
        {}

private:
        // typedefs

        typedef Node* node_ptr;
        typedef T value_type;

        // modifiers

        void do_attach(node_ptr other)
        {
                next_ = other;
        }

        // queries

        node_ptr do_next() const
        {
                return next_;
        }

        value_type const& do_value() const
        {
                return value_;
        }

        value_type& do_value()
        {
                return value_;
        }

        // representation

        node_ptr next_;
        value_type value_;
};

}       // namespace tree
}       // namespace dctl
