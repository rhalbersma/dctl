#pragma once
#include <cstddef>                              // ptrdiff_t
#include <cstdint>                              // uint64_t
#include <iterator>                             // iterator
#include <type_traits>                          // enable_if, is_unsigned, is_convertible
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/iterator/iterator_facade.hpp>   // iterator_facade, iterator_core_acces
#include <dctl/bit/iterator_fwd.hpp>            // bit_iterator, bit_reference
#include <dctl/bit/raw.hpp>                     // empty, pop_front, front

namespace dctl {
namespace bit {

template<class T, class U, class /* Requires */>
class bit_iterator
:
        public boost::iterator_facade<
                bit_iterator<T, U>,
                T,
                std::forward_iterator_tag,
                bit_reference<T, U>,
                std::ptrdiff_t
        >
{
public:
        // structors

        bit_iterator()
        :
                mask_(0)
        {}

        explicit bit_iterator(U m)
        :
                mask_(m)
        {}

        // yes, we really want implicit conversion from other iterators
        template<class V, class W, class Requires = typename std::enable_if<std::is_convertible<V, T>::value && std::is_convertible<W, U>::value>::type>
        bit_iterator(bit_iterator<V, W> other)
        :
                mask_(other->mask_)
        {}

private:
        friend class boost::iterator_core_access;
        template<class, class, class> friend class bit_iterator;

        // modifiers

        // operator++(), operator++(int) provided by boost::iterator_facade
        void increment()
        {
                // cannot increment a null pointer
                BOOST_ASSERT(!is_null());
                pop_front(mask_);
        }

        // views

        // operator* provided by boost::iterator_facade
        bit_reference<T, U> dereference() const
        {
                // cannot dereference a null pointer
                BOOST_ASSERT(!is_null());
                return bit_reference<T, U>(mask_);
        }

        // predicates

        // operator==, operator!= provided by boost::iterator_facade
        bool equal(bit_iterator const& other) const
        {
                return this->mask_ == other.mask_;
        }

        bool is_null() const
        {
                return empty(mask_);
        }

        // representation

        U mask_;
};

template<class T, class U, class /* Requires */>
class bit_reference
{
public:
        // structors

        // references have to be initialized
        bit_reference() = delete;

        // references cannot be null
        explicit bit_reference(U m)
        :
                mask_(m)
        {
                BOOST_ASSERT(!is_null());
        }

        // modifiers

        // references cannot be rebound
        bit_reference& operator=(bit_reference const&) = delete;

        // views

        // yes, we really want implicit conversion to the value type
        operator T() const
        {
                return static_cast<T>(front(mask_));
        }

        bit_iterator<T, U> operator&() const
        {
                return bit_iterator<T, U>(mask_);
        }

private:
        // predicates

        bool is_null() const
        {
                // references cannot be null
                return empty(mask_);
        }

        // representation

        U mask_;
};

}       // namespace bit
}       // namespace dctl
