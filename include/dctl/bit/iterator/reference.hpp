#pragma once
#include <dctl/bit/iterator/iterator_fwd.hpp>   // ConstIterator
#include <dctl/bit/iterator/reference_fwd.hpp>  // ConstReference
#include <dctl/bit/limits.hpp>                  // digits, is_unsigned_integer
#include <cassert>                              // assert

namespace dctl {
namespace bit {

template<class Block, int Nb, int N>
class ConstReference
{
        static_assert(is_unsigned_integer<Block>, "");
        static_assert(N <= Nb * digits<Block>, "");

public:
        // constructors

        // references cannot be left uninitialized
        ConstReference() = delete;

        constexpr ConstReference(Block const& b, int n) noexcept
        :
                block_{b},
                index_{n}
        {
                assert(0 <= n & n < N);
        }

        // copying and assignment

        // references can be copied
        constexpr ConstReference(ConstReference const&) = default;

        // references cannot be assigned to
        ConstReference& operator=(ConstReference const&) = delete;

        // const references cannot be assigned through
        ConstReference& operator=(int) = delete;

        // observers

        constexpr ConstIterator<Block, Nb, N> operator&() const noexcept
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
