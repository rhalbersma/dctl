#pragma once
#include "link/ISingle.hpp"

namespace dctl {
namespace tree {

template
<
        typename,
        typename
> 
struct Node;

template<typename T>
struct Node<link::single_tag, T>
:
        // Curiously Recurring Template Pattern (CRTP)
        public link::ISingle<Node, link::single_tag, T>
{
public:
        // structors

        Node(T const& v)
        :
                next_(nullptr),
                value_(v)
        {}

private:
        // typdefs

        typedef Node* node_ptr;

        // modifiers

        void do_attach(node_ptr other)
        {
                next_ = other;
        }

        void do_detach()
        {
                next_ = nullptr;
        }

        // queries

        node_ptr do_next() const
        {
                return next_;
        }

        // representation

        node_ptr next_;

public:
        T value_;
};

}       // namespace tree
}       // namespace dctl
