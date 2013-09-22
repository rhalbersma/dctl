#pragma once
#include <cstdint>                              // uint64_t
#include <dctl/bit/iterator_fwd.hpp>            // bit_iterator
#include <dctl/bit/reference_fwd.hpp>           // bit_reference

namespace dctl {
namespace bit {

template<class T, int Nw>
class bit_reference
{
private:
        // structors

        constexpr bit_reference(T const& s, int i) noexcept
        :
                segment_{s},
                index_{i}
        {}

        friend class bit_iterator<T, Nw>;

        // modifiers

        bit_reference& operator=(bit_reference const&) = delete;

public:
        // views

        constexpr operator int() const noexcept
        {
                return index_;
        }

        constexpr auto operator&() const noexcept -> bit_iterator<T, Nw>
        {
                return {&segment_, index_};
        }

private:
        // representation

        T const& segment_;
        int index_;
};

}       // namespace bit
}       // namespace dctl
