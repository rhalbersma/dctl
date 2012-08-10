#pragma once
#include <iterator>                     // iterator, forward_iterator_tag
#include <type_traits>                  // is_base_of
#include <boost/config.hpp>             // BOOST_ASSERT
#include <boost/operators.hpp>          // equality_comparable
#include "Iterator_fwd.hpp"
#include "../link/Single.hpp"

namespace dctl {
namespace tree {
namespace iterator {

template<typename T>
struct Iterator<T, link::Single>
:
        public std::iterator<std::forward_iterator_tag, T>,
        private boost::equality_comparable< Iterator<T, link::Single> >
{
        typedef link::Single link_type;

        // structors

        Iterator()
        :
                link_()
        {}

        explicit Iterator(link_type* it)
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

        bool empty() const
        {
                return link_ != nullptr;
        }
};

// TODO: use C++11 template aliases
template<T>
struct Forward
:
        Iterator<T, link::Single>
{};

}       // namespace iterator
}       // namespace tree
}       // namespace dctl
