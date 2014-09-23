#pragma once
#include <dctl/bitset/detail/base_bitset_fwd.hpp>       // base_bitset
#include <dctl/bitset/intrinsic.hpp>                    // popcount
#include <dctl/bitset/limits.hpp>                       // digits, is_unsigned_integer
#include <dctl/bitset/masks.hpp>                        // none, one, all
#include <cassert>                                      // assert
#include <utility>                                      // swap

namespace xstd {
namespace detail {

template<class Block, int Nb>
class base_bitset
{
private:
        static_assert(is_unsigned_integer<Block>, "");
        static constexpr auto N = Nb * digits<Block>;

public:
        // constructors

        constexpr base_bitset() = default;

        /* implicit */ constexpr base_bitset(Block value) noexcept
        :
                elems{value}
        {}

protected:
        // destructor

        ~base_bitset() = default;

public:
        // copying and assignment

        base_bitset(base_bitset const&) = default;
        base_bitset(base_bitset&&) = default;
        base_bitset& operator=(base_bitset const&) = default;
        base_bitset& operator=(base_bitset&&) = default;

        // data access

        constexpr auto* block_begin() noexcept
        {
                return elems;
        }

        constexpr auto const* block_begin() const noexcept
        {
                return elems;
        }

        constexpr auto* block_end() noexcept
        {
                return elems + Nb;
        }

        constexpr auto const* block_end() const noexcept
        {
                return elems + Nb;
        }

        constexpr auto& block_back() noexcept
        {
                return elems[Nb - 1];
        }

        constexpr auto const& block_back() const noexcept
        {
                return elems[Nb - 1];
        }

        constexpr auto& block_ref(int n)
        {
                assert(0 <= n && n < N);
                return elems[n / digits<Block>];
        }

        constexpr auto const& block_ref(int n) const
        {
                assert(0 <= n && n < N);
                return elems[n / digits<Block>];
        }

        // comparators

        constexpr auto do_equal(base_bitset const& other) const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (elems[i] != other.elems[i])
                                return false;
                return true;
        }

        constexpr auto do_colexicographical_compare(base_bitset const& other) const noexcept
        {
                for (auto i = Nb - 1; i >= 0; --i) {
                        if (elems[i] < other.elems[i])
                                return true;
                        if (elems[i] > other.elems[i])
                                return false;
                }
                return false;
        }

        constexpr auto do_intersects(base_bitset const& other) const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (elems[i] & other.elems[i])
                                return true;
                return false;
        }

        constexpr auto do_is_subset_of(base_bitset const& other) const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (elems[i] & ~other.elems[i])
                                return false;
                return true;
        }

        constexpr auto do_is_proper_subset_of(base_bitset const& other) const noexcept
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

        /* constexpr */ auto do_swap(base_bitset& other) noexcept
        {
                using std::swap;
                swap(elems, other.elems);
        }

        constexpr auto do_sanitize() noexcept {}

        constexpr auto do_set() noexcept
        {
                for (auto&& block : elems)
                        block = masks::all<Block>;
        }

        constexpr auto do_reset() noexcept
        {
                for (auto&& block : elems)
                        block = masks::none<Block>;
        }

        constexpr auto do_flip() noexcept
        {
                for (auto&& block : elems)
                        block = ~block;
        }

        constexpr auto do_and(base_bitset const& other) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        elems[i] &= other.elems[i];
        }

        constexpr auto do_or(base_bitset const& other) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        elems[i] |= other.elems[i];
        }

        constexpr auto do_xor(base_bitset const& other) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        elems[i] ^= other.elems[i];
        }

        constexpr auto do_minus(base_bitset const& other) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        elems[i] &= ~other.elems[i];
        }

        constexpr auto do_left_shift(int n)
        {
                assert(0 <= n && n < N);
                if (n == 0) return;

                auto const n_block = n / digits<Block>;
                auto const L_shift = n % digits<Block>;

                if (L_shift == 0) {
                        for (auto i = Nb - 1; i >= n_block; --i)
                                elems[i] = elems[i - n_block];
                } else {
                        auto const R_shift = digits<Block> - L_shift;

                        for (auto i = Nb - 1; i > n_block; --i)
                                elems[i] =
                                        (elems[i - n_block    ] << L_shift) |
                                        (elems[i - n_block - 1] >> R_shift)
                                ;
                        elems[n_block] = elems[0] << L_shift;
                }
                for (auto i = n_block - 1; i >= 0; --i)
                        elems[i] = masks::none<Block>;
        }

        constexpr auto do_right_shift(int n)
        {
                assert(0 <= n && n < N);
                if (n == 0) return;

                auto const n_block = n / digits<Block>;
                auto const R_shift = n % digits<Block>;

                if (R_shift == 0) {
                        for (auto i = 0; i <= Nb - 1 - n_block; ++i)
                               elems[i] = elems[i + n_block];
                } else {
                        auto const L_shift = digits<Block> - R_shift;

                        for (auto i = 0; i < Nb - 1 - n_block; ++i)
                                elems[i] =
                                        (elems[i + n_block    ] >> R_shift) |
                                        (elems[i + n_block + 1] << L_shift)
                                ;
                        elems[Nb - 1 - n_block] = elems[Nb - 1] >> R_shift;
                }
                for (auto i = Nb - n_block; i < Nb; ++i)
                        elems[i] = masks::none<Block>;
        }

        // observers

        constexpr auto do_all() const noexcept
        {
                for (auto&& block : elems)
                        if (block != masks::all<Block>)
                                return false;
                return true;
        }

        constexpr auto do_any() const noexcept
        {
                for (auto&& block : elems)
                        if (block != masks::none<Block>)
                                return true;
                return false;
        }

        constexpr auto do_none() const noexcept
        {
                for (auto&& block : elems)
                        if (block != masks::none<Block>)
                                return false;
                return true;
        }

        constexpr auto do_count() const noexcept
        {
                auto sum = 0;
                for (auto&& block : elems)
                        sum += intrinsic::popcount(block);
                return sum;
        }

 private:
        // representation

        Block elems[Nb]{};
};

}       // namespace detail
}       // namespace xstd
