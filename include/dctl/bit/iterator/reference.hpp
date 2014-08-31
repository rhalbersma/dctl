#pragma once
#include <dctl/bit/iterator/iterator_fwd.hpp>   // ConstIterator
#include <dctl/bit/iterator/reference_fwd.hpp>  // ConstReference
#include <dctl/bit/traits.hpp>                  // is_unsigned_integer

namespace dctl {
namespace bit {

template<class Block, int Nb>
class ConstReference
{
        static_assert(
                is_unsigned_integer<Block>,
                "Template parameter 'T' in 'ConstReference<T, N>' shall be of unsigned integer type."
        );

public:
        // constructors

        // references cannot be left uninitialized
        ConstReference() = delete;

        constexpr ConstReference(Block const& b, int n) noexcept
        :
                block_{b},
                index_{n}
        {}

        // copying and assignment

        // references can be copied
        constexpr ConstReference(ConstReference const&) = default;

        // references cannot be assigned to
        ConstReference& operator=(ConstReference const&) = delete;

        // const references cannot be assigned through
        ConstReference& operator=(int) = delete;

        // observers

        constexpr ConstIterator<Block, Nb> operator&() const noexcept
        {
                return { &block_, index_ };
        }

        /* implicit */ constexpr operator int() const noexcept
        {
                return index_;
        }

private:
        // representation

        Block const& block_;
        int index_;
};

}       // namespace bit
}       // namespace dctl
