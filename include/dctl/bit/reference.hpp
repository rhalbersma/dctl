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

        constexpr bit_reference(storage_type const& s, int i) noexcept
        :
                segment_{s},
                index_{i}
        {}

        // modifiers

        bit_reference& operator=(bit_reference const&) = delete;

        // views

        constexpr operator int() const noexcept
        {
                return index_;
        }

        constexpr auto operator&() const noexcept -> bit_iterator<1, _>
        {
                return {&segment_, index_};
        }

private:
        // representation

        storage_type const& segment_;
        int index_;
};

}       // namespace bit
}       // namespace dctl
