#pragma once
#include <iterator>                     // iterator, forward_iterator_tag
#include <type_traits>                  // is_base_of
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/operators.hpp>          // equality_comparable
#include "Iterator_fwd.hpp"
#include "../link/ISingle.hpp"

namespace dctl {
namespace tree {
namespace iterator {

template<template<typename, typename> class Node, typename T>
struct Iterator<Node, link::single_tag, T>
:
        // Curiously Recurring Template Pattern (CRTP)
        public std::iterator<std::forward_iterator_tag, T>,
        private boost::equality_comparable< Iterator<Node, link::single_tag, T> >
{
public:
        // typedefs

        typedef Node<link::ISingle, T>* node_ptr;

        // structors

        Iterator()
        :
                pnode_()
        {}

        explicit Iterator(node_ptr it)
        :
                pnode_(it)
        {}

        // modifiers

        Iterator& operator++()
        {
                pnode_ = pnode_->next();
                return *this;
        }

        Iterator operator++(int)
        {
                auto const old = *this;
                ++*this;
                return old;
        }

        // queries

        reference operator*() const
        {
                return pnode_->value_;
        }

        pointer operator->() const
        {
                return &**this;
        }

        // predicates

        friend bool operator==(Iterator const& lhs, Iterator const& rhs)
        {
                return lhs.pnode_ == rhs.pnode_;
        }

private:
        // representation

        node_ptr pnode_;
};

// TODO: use C++11 template aliases
template<template<typename, typename> class Node, typename T>
struct Forward
:
        Iterator<Node, link::single_tag, T>
{};

}       // namespace iterator
}       // namespace tree
}       // namespace dctl
