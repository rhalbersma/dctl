#pragma once
#include <cstddef>                              // ptrdiff_t
#include <cstdint>                              // uint64_t
#include <type_traits>                          // enable_if, is_unsigned, is_convertible
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <dctl/bit/iterator_fwd.hpp>            // bit_iterator
#include <dctl/bit/reference_fwd.hpp>           // bit_reference
#include <dctl/bit/raw.hpp>                     // empty, pop_front, front

namespace dctl {
namespace bit {

template<class Key>
class bit_reference<Key, uint64_t>
{
public:
        using storage_type = uint64_t;

        // structors

        // references have to be initialized
        bit_reference() = delete;

        // references cannot be null
        explicit bit_reference(storage_type m)
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
        operator Key() const
        {
                return Key{front(mask_)};
        }

        bit_iterator<Key, storage_type> operator&() const
        {
                return bit_iterator<Key, storage_type>{mask_};
        }

private:
        // predicates

        bool is_null() const
        {
                // references cannot be null
                return empty(mask_);
        }

        // representation

        storage_type mask_;
};

}       // namespace bit
}       // namespace dctl
