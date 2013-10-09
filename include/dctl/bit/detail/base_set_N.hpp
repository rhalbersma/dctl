#pragma once
#include <array>                                // array
#include <limits>                               // digits
#include <dctl/bit/detail/base_set_fwd.hpp>     // base_set
#include <dctl/bit/detail/storage.hpp>          // storage
#include <dctl/bit/intrinsic.hpp>               // popcount

namespace dctl {
namespace bit {
namespace detail {

template<class T, class Block, int Nb>
struct base_set
{
        // structors

        constexpr base_set() noexcept = default;

        // element access

        constexpr auto block_ptr(T n) noexcept
        {
                return &data_[0] + storage<Block>::block_idx(n);
        }

        constexpr auto block_ptr(T n) const noexcept
        {
                return &data_[0] + storage<Block>::block_idx(n);
        }

        // bitwise operations

        constexpr void do_reset() noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        data_[i] = Block{0};
        }

        constexpr void do_set() noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        data_[i] = ~Block{0};
        }

        constexpr void do_flip() noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        data_[i] = ~data_[i];
        }

        constexpr void do_and(base_set const& other) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        data_[i] &= other.data_[i];
        }

        constexpr void do_or(base_set const& other) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        data_[i] |= other.data_[i];
        }

        constexpr void do_xor(base_set const& other) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        data_[i] ^= other.data_[i];
        }

        constexpr void do_left_shift(int n)
        {
                if (n == 0) return;

                auto const n_block = storage<Block>::block_idx(n);
                auto const L_shift = storage<Block>::shift_idx(n);

                if (L_shift == 0) {
                        for (auto i = Nb - 1; i >= n_block; --i)
                                data_[i] = data_[i - n_block];
                } else {
                        auto const R_shift = std::numeric_limits<Block>::digits - L_shift;

                        for (auto i = Nb - 1; i > n_block; --i)
                                data_[i] =
                                        data_[i - n_block    ] << L_shift |
                                        data_[i - n_block - 1] >> R_shift
                                ;
                        data_[n_block] = data_[0] << L_shift;
                }

                for (auto i = n_block - 1; i >= 0; --i)
                        data_[i] = Block{0};
        }

        constexpr void do_right_shift(int n)
        {
                if (n == 0) return;

                auto const n_block = storage<Block>::block_idx(n);
                auto const R_shift = storage<Block>::shift_idx(n);

                if (R_shift == 0) {
                        for (auto i = 0; i < Nb - n_block; ++i)
                                data_[i] = data_[i + n_block];
                } else {
                        auto const L_shift = std::numeric_limits<Block>::digits - R_shift;

                        for (auto i = 0; i < Nb - n_block - 1; ++i)
                                data_[i] =
                                        data_[i + n_block    ] >> R_shift |
                                        data_[i + n_block + 1] << L_shift
                                ;
                        data_[Nb - n_block - 1] = data_[Nb] >> R_shift;
                }

                for (auto i = Nb - n_block; i < Nb; ++i)
                        data_[i] = Block{0};
        }

        // bitwise algorithms

        static constexpr auto do_equal(base_set const& lhs, base_set const& rhs) noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (lhs.data_[i] != rhs.data_[i])
                                return false;
                return true;
        }

        static constexpr auto do_lexicographical_compare(base_set const& lhs, base_set const& rhs) noexcept
        {
                for (auto i = 0; i < Nb; ++i) {
                        if (lhs.data_[i] < rhs.data_[i]) return true;
                        if (rhs.data_[i] < lhs.data_[i]) return false;
                }
                return false;
        }

        constexpr auto do_includes(base_set const& other) const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if ((other.data_[i] & ~data_[i]) != Block{0})
                                return false;
                return true;
        }

        constexpr auto do_intersects(base_set const& other) const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if ((data_ & other.data_) != Block{0})
                                return true;
                return false;
        }

        constexpr auto do_none() const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (data_[i] != Block{0})
                                return false;
                return true;
        }

        constexpr auto do_any() const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (data_[i] != Block{0})
                                return true;
                return false;
        }

        constexpr auto do_all() const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (data_[i] != ~Block{0})
                                return false;
                return true;
        }

        constexpr auto do_count_equal_to(int n) const noexcept
        {
                auto sum = 0;
                for (auto i = 0; i < Nb; ++i) {
                        sum += intrinsic::popcount(data_[i]);
                        if (sum > n)
                                return false;
                }
                return sum == n;
        }

        constexpr auto do_count_less(int n) const noexcept
        {
                auto sum = 0;
                for (auto i = 0; i < Nb; ++i) {
                        sum += intrinsic::popcount(data_[i]);
                        if (sum >= n)
                                return false;
                }
                return true;
        }

        constexpr auto do_count() const noexcept
        {
                auto sum = 0;
                for (auto i = 0; i < Nb; ++i)
                        sum += intrinsic::popcount(data_[i]);
                return sum;
        }

        // representation

        std::array<Block, Nb> data_ {};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
