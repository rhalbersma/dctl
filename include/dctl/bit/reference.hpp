#pragma once
#include <type_traits>                          // is_convertible
#include <dctl/bit/iterator_fwd.hpp>            // bit_iterator
#include <dctl/bit/reference_fwd.hpp>           // bit_reference

namespace dctl {
namespace bit {

template<class T, class Block, int Nb>
class bit_reference
{
public:
        using iterator = bit_iterator<T, Block, Nb>;

        // structors

        template<class U>
        constexpr bit_reference(Block const& b, U value) noexcept
        :
                block_{b},
                index_{static_cast<int>(value)}
        {
                static_assert(std::is_convertible<U, int>::value, "");
        }

        // views

        constexpr operator T() const noexcept
        {
                static_assert(std::is_convertible<int, T>::value, "");
                return static_cast<T>(index_);
        }

        constexpr iterator operator&() const noexcept
        {
                return {&block_, index_};
        }

private:
        // representation

        Block const& block_;
        int index_;
};

}       // namespace bit
}       // namespace dctl
