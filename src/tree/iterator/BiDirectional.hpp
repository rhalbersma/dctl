#pragma once
#include <iterator>                     // iterator, bidirectional_iterator_tag
#include <type_traits>                  // is_base_of
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/operators.hpp>          // equality_comparable
#include "Iterator_fwd.hpp"
#include "../link/Double.hpp"

namespace dctl {
namespace tree {
namespace iterator {

template<typename T>
struct Iterator<T, link::Double>
:
        public std::iterator<std::bidirectional_iterator_tag, T>,
        private boost::equality_comparable< Iterator<T, link::Double> >
{
        typedef link::Double link_type;

        // structors

        Iterator(link_type* it)
        :
                link_(it)
        {}

        // modifiers

        Iterator& operator++()
        {
                link_ = link_->next_;
                return *this;
        }

        Iterator operator++(int)
        {
                auto tmp(*this);
                ++*this;
                return tmp;
        }

        Iterator& operator--()
        {
                link_ = link_->prev_;
                return *this;
        }

        Iterator operator--(int)
        {
                auto tmp(*this);
                ++*this;
                return tmp;
        }

        // queries

        reference operator*() const
        {
                return node()->value_;
        }

        pointer operator->() const
        {
                return &**this;
        }

        // predicates

        bool operator==(Iterator const& other) const
        {
                return link_ == other.link_;
        }

        // representation

        link_type* link_;

private:
        typedef Node<T, link_type> node_type;

        // implementation

        node_type* node() const
        {
                BOOST_MPL_ASSERT((std::is_base_of<link_type, node_type>));
                return static_cast<node_type*>(this);
        }
};

// TODO: use C++11 template aliases
template<T>
struct Bidirectional
:
        Iterator<T, link::Double>
{};

}       // namespace iterator
}       // namespace tree
}       // namespace dctl
