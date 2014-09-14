#pragma once
#include <dctl/bit/detail/base_set_fwd.hpp>     // BaseSet
#include <dctl/bit/intrinsic.hpp>               // popcount
#include <dctl/bit/traits.hpp>                  // none, one, all, digits, is_unsigned_integer
#include <cassert>                              // assert
#include <utility>                              // swap

namespace dctl {
namespace bit {
namespace detail {

template<int Nb>
class BaseSet
{
private:
        using block_type = unsigned long long;
        static constexpr auto N = Nb * digits<block_type>;

public:
        // constructors

        constexpr BaseSet() = default;

        /* implicit */ constexpr BaseSet(unsigned long long value) noexcept
        :
                elems{value}
        {}

protected:
        // destructor

        ~BaseSet() = default;

public:
        // copying and assignment

        BaseSet(BaseSet const&) = default;
        BaseSet& operator=(BaseSet const&) = default;

        // element access

        constexpr auto* block_ptr(int n)
        {
                assert(0 <= n && n <= N);
                return elems + n / digits<block_type>;
        }

        constexpr auto const* block_ptr(int n) const
        {
                assert(0 <= n && n <= N);
                return elems + n / digits<block_type>;
        }

        // data access

        constexpr auto* do_data()
        {
                return elems;
        }

        constexpr auto const* do_data() const
        {
                return elems;
        }

        // comparators

        constexpr auto do_equal(BaseSet const& other) const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (elems[i] != other.elems[i])
                                return false;
                return true;
        }

        constexpr auto do_colexicographical_compare(BaseSet const& other) const noexcept
        {
                for (auto i = Nb - 1; i >= 0; --i) {
                        if (elems[i] < other.elems[i])
                                return true;
                        if (elems[i] > other.elems[i])
                                return false;
                }
                return false;
        }

        constexpr auto do_intersects(BaseSet const& other) const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (elems[i] & other.elems[i])
                                return true;
                return false;
        }

        constexpr auto do_is_subset_of(BaseSet const& other) const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (elems[i] & ~other.elems[i])
                                return false;
                return true;
        }

        constexpr auto do_is_proper_subset_of(BaseSet const& other) const noexcept
        {
                auto proper = false;
                for (auto i = 0; i < Nb; ++i) {
                        if ( elems[i] & ~other.elems[i])
                                return false;
                        if (~elems[i] &  other.elems[i])
                                proper = true;
                }
                return proper;
        }

        // modifiers

        /* constexpr */ auto do_swap(BaseSet& other) noexcept
        {
                using std::swap;
                swap(elems, other.elems);
        }

        constexpr auto do_sanitize() noexcept {}

        constexpr auto do_set() noexcept
        {
                for (auto&& block : elems)
                        block = all<block_type>;
        }

        constexpr auto do_reset() noexcept
        {
                for (auto&& block : elems)
                        block = none<block_type>;
        }

        constexpr auto do_flip() noexcept
        {
                for (auto&& block : elems)
                        block = ~block;
        }

        constexpr auto do_and(BaseSet const& other) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        elems[i] &= other.elems[i];
        }

        constexpr auto do_or(BaseSet const& other) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        elems[i] |= other.elems[i];
        }

        constexpr auto do_xor(BaseSet const& other) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        elems[i] ^= other.elems[i];
        }

        constexpr auto do_minus(BaseSet const& other) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        elems[i] &= ~other.elems[i];
        }

        constexpr auto do_left_shift(int n)
        {
                assert(0 <= n && n < N);
                if (n == 0) return;

                auto const n_block = n / digits<block_type>;
                auto const L_shift = n % digits<block_type>;

                if (L_shift == 0) {
                        for (auto i = Nb - 1; i >= n_block; --i)
                                elems[i] = elems[i - n_block];
                } else {
                        auto const R_shift = digits<block_type> - L_shift;

                        for (auto i = Nb - 1; i > n_block; --i)
                                elems[i] =
                                        (elems[i - n_block    ] << L_shift) |
                                        (elems[i - n_block - 1] >> R_shift)
                                ;
                        elems[n_block] = elems[0] << L_shift;
                }
                for (auto i = n_block - 1; i >= 0; --i)
                        elems[i] = none<block_type>;
        }

        constexpr auto do_right_shift(int n)
        {
                assert(0 <= n && n < N);
                if (n == 0) return;

                auto const n_block = n / digits<block_type>;
                auto const R_shift = n % digits<block_type>;

                if (R_shift == 0) {
                        for (auto i  = 0; i <= Nb - 1 - n_block; ++i)
                               elems[i] = elems[i + n_block];
                } else {
                        auto const L_shift = digits<block_type> - R_shift;

                        for (auto i = 0; i < Nb - 1 - n_block; ++i)
                                elems[i] =
                                        (elems[i + n_block    ] >> R_shift) |
                                        (elems[i + n_block + 1] << L_shift)
                                ;
                        elems[Nb - 1 - n_block] = elems[Nb - 1] >> R_shift;
                }
                for (auto i = Nb - n_block; i < Nb; ++i)
                        elems[i] = none<block_type>;
        }

        // observers

        constexpr auto do_all() const noexcept
        {
                for (auto&& block : elems)
                        if (block != all<block_type>)
                                return false;
                return true;
        }

        constexpr auto do_any() const noexcept
        {
                for (auto&& block : elems)
                        if (block != none<block_type>)
                                return true;
                return false;
        }

        constexpr auto do_none() const noexcept
        {
                for (auto&& block : elems)
                        if (block != none<block_type>)
                                return false;
                return true;
        }

        constexpr auto do_count() const noexcept
        {
                auto sum = 0;
                for (auto&& block : elems)
                        sum += bit::intrinsic::popcount(block);
                return sum;
        }

 private:
        // representation

        block_type elems[Nb]{};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
