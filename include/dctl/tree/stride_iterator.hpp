#pragma once
#include <cstddef>                      // size_t
#include <iterator>                     // advance, forward_iterator_tag, iterator, iterator_traits
#include <type_traits>                  // is_base_of, is_copy_constructible, is_default_constructible
#include <boost/operators.hpp>          // equality_comparable1

namespace dctl {
namespace tree {

template
<
        class ForwardIterator,
        std::size_t N = 2
>
struct stride_forward_iterator
        // http://www.boost.org/doc/libs/1_52_0/libs/utility/operators.htm#chaining
        // use base class chaining to ensure Empty Base Optimization
:       boost::equality_comparable1< stride_forward_iterator<ForwardIterator, N> // == !=
,       std::iterator<std::forward_iterator_tag, typename std::iterator_traits<ForwardIterator>::value_type>
        >
{
private:
        // dependent names now in scope

        using Base = std::iterator<std::forward_iterator_tag, typename std::iterator_traits<ForwardIterator>::value_type>;
        using typename Base::pointer;    // T*
        using typename Base::reference;  // T&

        using self_type = stride_forward_iterator;

public:
        // constructors

        stride_forward_iterator()
        :
                fwd_()
        {}

        stride_forward_iterator(ForwardIterator it)
        :
                fwd_(it)
        {}

        // modifiers

        self_type& operator++()
        {
                std::advance(fwd_, N);
                return *this;
        }

        self_type operator++(int)
        {
                auto const old = *this;
                ++*this;
                return old;
        }

        // observers

        reference operator*() const
        {
                return *fwd_;
        }

        pointer operator->() const
        {
                return &**this;
        }

        // predicates

        // operator!= provided by boost::equality_comparable1
        friend bool operator==(self_type const& lhs, self_type const& rhs)
        {
                return lhs.fwd_ == rhs.fwd_;
        }

private:
        // representation

        ForwardIterator fwd_;
};

}       // namespace tree
}       // namespace dctl
