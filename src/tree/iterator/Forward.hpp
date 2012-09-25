#pragma once
#include <iterator>                     // iterator, forward_iterator_tag
#include <type_traits>                  // is_base_of
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/operators.hpp>          // equality_comparable

namespace dctl {
namespace tree {
namespace iterator {

template
<
        template<typename> class Node,
        typename T
>
struct Forward
:
        // Curiously Recurring Template Pattern (CRTP)
        public std::iterator<std::forward_iterator_tag, T>,
        private boost::equality_comparable< Forward<Node, T> >
{
public:
        // typedefs

        typedef Node<T>* node_ptr;

        // structors

        Forward()
        :
                pnode_()
        {}

        explicit Forward(node_ptr fwd)
        :
                pnode_(fwd)
        {}

        // modifiers

        Forward& operator++()
        {
                pnode_ = pnode_->next();
                return *this;
        }

        Forward operator++(int)
        {
                auto const old = *this;
                ++*this;
                return old;
        }

        // queries

        reference operator*() const
        {
                return pnode_->value();
        }

        pointer operator->() const
        {
                return &**this;
        }

        // predicates

        friend bool operator==(Forward const& lhs, Forward const& rhs)
        {
                return lhs.pnode_ == rhs.pnode_;
        }

private:
        // representation

        node_ptr pnode_;
};

}       // namespace iterator
}       // namespace tree
}       // namespace dctl
