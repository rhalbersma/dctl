#pragma once
#include <dctl/tree/link/i_single.hpp>  // ISingle

namespace dctl {
namespace tree {

template<typename T>
struct Node
:
        // Curiously Recurring Template Pattern (CRTP)
        public link::ISingle< Node<T> >
{
public:
        friend class ISingle< Node >;

        // typedefs

        typedef Node* node_ptr;
        typedef T value_type;
        typedef value_type& reference;
        typedef value_type const& const_reference;

        // structors

        Node(node_ptr p, T const& v)
        :
                value_(v),
                next_(p)
        {}

private:
        // modifiers

        void do_attach(node_ptr other)
        {
                next_ = other;
        }

        reference do_value()
        {
                return value_;
        }

        // queries

        const_reference do_value() const
        {
                return value_;
        }

        node_ptr do_next() const
        {
                return next_;
        }

        // representation

        value_type value_;
        node_ptr next_;
};

}       // namespace tree
}       // namespace dctl
