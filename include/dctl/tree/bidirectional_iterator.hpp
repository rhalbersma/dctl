#pragma once
#include <iterator>                             // bidirectional_iterator_tag, iterator, iterator_traits, next, prev
#include <type_traits>                          // enable_if, is_same
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/operators.hpp>                  // equality_comparable1
#include <dctl/tree/bidirectional_link.hpp>     // bidirectional_link
#include <dctl/tree/node.hpp>                   // node

namespace dctl {
namespace tree {

template<typename T>
struct bidirectional_iterator
        // http://www.boost.org/doc/libs/1_52_0/libs/utility/operators.htm#chaining
        // use base class chaining to ensure Empty Base Optimization
:       boost::equality_comparable1< bidirectional_iterator<T>      // == !=
,       std::iterator<std::bidirectional_iterator_tag, T>
        >
{
private:
        // dependent names now in scope

        typedef std::iterator<std::bidirectional_iterator_tag, T> Base;
        using typename Base::value_type; // T
        using typename Base::pointer;    // T*
        using typename Base::reference;  // T&

        // typedefs

        typedef bidirectional_iterator self_type;
        typedef bidirectional_link link_type;
public:
        typedef node<link_type, value_type> node_type;
private:
        typedef link_type* link_ptr;
        typedef node_type* node_ptr;

public:
        // structors

        bidirectional_iterator()
        :
                plink_()
        {}

        explicit bidirectional_iterator(link_ptr p)
        :
                plink_(p)
        {}

        // modifiers

        self_type& operator++()
        {
                plink_ = plink_->next();
                return *this;
        }

        self_type operator++(int)
        {
                auto const old = *this;
                ++*this;
                return old;
        }

        self_type& operator--()
        {
                plink_ = plink_->prev();
                return *this;
        }

        self_type operator--(int)
        {
                auto const old = *this;
                --*this;
                return old;
        }

        friend void set_successor(self_type& lhs, self_type& rhs)
        {
                set_successor(lhs.plink_, rhs.plink_);
                BOOST_ASSERT(is_successor(lhs, rhs));
        }

        // queries

        reference operator*() const
        {
                return static_cast<node_ptr>(plink_)->value_;
        }

        pointer operator->() const
        {
                return &**this;
        }

        // predicates

        // operator!= provided by boost::equality_comparable1
        friend bool operator==(self_type const& lhs, self_type const& rhs)
        {
                return lhs.plink_ == rhs.plink_;
        }

        friend bool is_successor(self_type const& lhs, self_type const& rhs)
        {
                return (
                        std::next(lhs) == rhs &&
                        std::prev(rhs) == lhs
                );
        }

private:
        // representation

        link_ptr plink_;
};

}       // namespace tree
}       // namespace dctl
