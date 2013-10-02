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
        constexpr bit_reference(Block const& s, U u) noexcept
        :
                block_{s},
                index_{static_cast<T>(u)}
        {
                static_assert(std::is_convertible<U, T>::value, "");
        }

        // modifiers

        bit_reference& operator=(bit_reference const&) = delete;

        // views

        constexpr operator T() const noexcept
        {
                return index_;
        }

        constexpr iterator operator&() const noexcept
        {
                return {&block_, index_};
        }

private:
        // representation

        Block const& block_;
        T index_;
};

}       // namespace bit
}       // namespace dctl
