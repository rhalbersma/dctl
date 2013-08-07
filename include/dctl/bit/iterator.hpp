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

template<class T>
class bit_iterator<T, 1>
:
        public boost::iterator_facade<
                bit_iterator<T, 1>,
                T,
                std::forward_iterator_tag,
                bit_reference<T, 1>,
                std::ptrdiff_t
        >
{
public:
        using block_type = uint64_t;

        // structors

        bit_iterator()
        :
                mask_{0}
        {}

        explicit bit_iterator(block_type m)
        :
                mask_{m}
        {}

private:
        friend class boost::iterator_core_access;

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
        bit_reference<T, 1> dereference() const
        {
                // cannot dereference a null pointer
                BOOST_ASSERT(!is_null());
                return bit_reference<T, 1>{mask_};
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

        block_type mask_;
};

template<class T>
class bit_reference<T, 1>
{
public:
        using block_type = uint64_t;

        // structors

        // references have to be initialized
        bit_reference() = delete;

        // references cannot be null
        explicit bit_reference(block_type m)
        :
                mask_{m}
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
                return T{front(mask_)};
        }

        bit_iterator<T, 1> operator&() const
        {
                return bit_iterator<T, 1>{mask_};
        }

private:
        // predicates

        bool is_null() const
        {
                // references cannot be null
                return empty(mask_);
        }

        // representation

        block_type mask_;
};

}       // namespace bit
}       // namespace dctl
