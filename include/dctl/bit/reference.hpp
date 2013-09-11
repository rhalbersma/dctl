#pragma once
#include <cstdint>                              // uint64_t
#include <dctl/bit/iterator_fwd.hpp>            // bit_iterator
#include <dctl/bit/reference_fwd.hpp>           // bit_reference

namespace dctl {
namespace bit {

template<class _>
class bit_reference<1, _>
{
public:
        using storage_type = uint64_t;

        // structors

        // references have to be initialized
        bit_reference() = delete;

        // references cannot be null
        bit_reference(storage_type const* s, int i)
        :
                segment_{s},
                index_{i}
        {}

        // modifiers

        // references cannot be rebound
        bit_reference& operator=(bit_reference const&) = delete;

        // views

        // yes, we really want implicit conversion to the value type
        operator int() const
        {
                return index_;
        }

        bit_iterator<1, _> operator&() const
        {
                return bit_iterator<1, _>{segment_, index_};
        }

private:
        // representation

        storage_type const* segment_;
        int index_;
};

}       // namespace bit
}       // namespace dctl
