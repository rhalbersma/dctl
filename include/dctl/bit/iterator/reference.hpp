#pragma once
#include <dctl/bit/iterator/iterator_fwd.hpp>   // ConstIterator
#include <dctl/bit/iterator/reference_fwd.hpp>  // ConstReference
#include <dctl/bit/traits.hpp>                  // digits, is_unsigned_integer
#include <cassert>                              // assert

namespace dctl {
namespace bit {

template<int N, int Nb>
class ConstReference
{
        using block_type = unsigned long long;
        static_assert(N <= Nb * digits<block_type>, "");

public:
        // constructors

        // references cannot be left uninitialized
        ConstReference() = delete;

        constexpr ConstReference(block_type const& b, int n) noexcept
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

        constexpr ConstIterator<N, Nb> operator&() const noexcept
        {
                return { &block_, index_ };
        }

        /* implicit */ constexpr operator int() const noexcept
        {
                return index_;
        }

private:
        // representation

        block_type const& block_;
        int index_;
};

}       // namespace bit
}       // namespace dctl
