#pragma once
#include <cstddef>                              // ptrdiff_t
#include <cstdint>                              // uint64_t
#include <iterator>                             // iterator
#include <type_traits>                          // enable_if, is_unsigned, is_convertible
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/iterator/iterator_facade.hpp>   // iterator_facade, iterator_core_acces
#include <dctl/bit/iterator_fwd.hpp>            // bit_iterator
#include <dctl/bit/reference_fwd.hpp>           // bit_reference
#include <dctl/bit/raw.hpp>                     // empty, pop_front, front

namespace dctl {
namespace bit {

template<class Key>
class bit_iterator<Key, uint64_t>
:
        public boost::iterator_facade<
                bit_iterator<Key, uint64_t>,
                Key,
                std::forward_iterator_tag,
                bit_reference<Key, uint64_t>,
                std::ptrdiff_t
        >
{
public:
        using storage_type = uint64_t;

        // structors

        bit_iterator() = default;

        explicit bit_iterator(storage_type m)
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
        bit_reference<Key, storage_type> dereference() const
        {
                // cannot dereference a null pointer
                BOOST_ASSERT(!is_null());
                return bit_reference<Key, storage_type>{mask_};
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

        storage_type mask_ = 0;
};

}       // namespace bit
}       // namespace dctl
