#pragma once
#include <type_traits>                          // is_convertible
#include <dctl/bit/iterator/iterator_fwd.hpp>   // Iterator
#include <dctl/bit/iterator/reference_fwd.hpp>  // Reference

namespace dctl {
namespace bit {

template<class T, class Block, int Nb>
class Reference
{
private:
        // types

        using iterator = Iterator<T, Block, Nb>;

public:
        // structors

        template<class U>
        constexpr Reference(Block const& b, U const& value) noexcept
        :
                block_{b},
                index_{value}
        {
                static_assert(std::is_convertible<U, int>::value, "");
        }

        // queries

        constexpr operator T() const noexcept
        {
                static_assert(std::is_convertible<int, T>::value, "");
                return T{index_};
        }

        constexpr iterator operator&() const noexcept
        {
                return { &block_, index_ };
        }

private:
        // representation

        Block const& block_;
        int index_;
};

}       // namespace bit
}       // namespace dctl
