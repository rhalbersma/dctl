#pragma once
#include <array>                                // array
#include <dctl/bit/detail/base_set_fwd.hpp>     // base_set
#include <dctl/bit/detail/storage.hpp>          // storage

namespace dctl {
namespace bit {
namespace detail {

template<class T, class Block, int Nb>
struct base_set
{
        using storage = storage<Block>;

        // structors

        constexpr base_set() noexcept = default;

        // element access

        constexpr auto block_ptr(T n) noexcept
        {
                return &data_[0] + storage::block(n);
        }

        constexpr auto block_ptr(T n) const noexcept
        {
                return &data_[0] + storage::block(n);
        }

        // bitwise operations

        constexpr void do_reset() noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        data_[i] = 0;
        }

        constexpr void do_set() noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        data_[i] = ~0;
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

        constexpr void do_left_shift(std::size_t n)
        {
                if (n == 0) return;

                auto const d_block = storage::block(n);
                auto const d_index = storage::index(n);

                if (d_index == 0) {
                        for (auto i = Nb - 1; i >= d_block; --i)
                                data_[i] = data_[i - d_block];
                } else {
                        auto const c_index = storage::size - d_index;

                        for (auto i = Nb - 1; i > d_block; --i)
                                data_[i] =
                                        data_[i - d_block    ] << d_index |
                                        data_[i - d_block - 1] >> c_index
                                ;
                        data_[d_block] = data_[0] << d_index;
                }

                for (auto i = d_block - 1; i >= 0; --i)
                        data_[i] = 0;
        }

        constexpr void do_right_shift(std::size_t n)
        {
                if (n == 0) return;

                auto const d_block = storage::block(n);
                auto const d_index = storage::index(n);

                if (d_index == 0) {
                        for (auto i = 0; i < Nb - d_block; ++i)
                                data_[i] = data_[i + d_block];
                } else {
                        auto const c_index = storage::size - d_index;

                        for (auto i = 0; i < Nb - d_block - 1; ++i)
                                data_[i] =
                                        data_[i + d_block    ] >> d_index |
                                        data_[i + d_block + 1] << c_index
                                ;
                        data_[Nb - d_block - 1] = data_[Nb] >> d_index;
                }

                for (auto i = Nb - d_block; i < Nb; ++i)
                        data_[i] = 0;
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

        constexpr auto do_none() const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (data_[i] != 0)
                                return false;
                return true;
        }

        constexpr auto do_any() const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (data_[i] != 0)
                                return true;
                return false;
        }

        constexpr auto do_all() const noexcept
        {
                for (auto i = 0; i < Nb; ++i)
                        if (data_[i] != ~0)
                                return false;
                return true;
        }

        constexpr auto do_count() const noexcept
        {
                auto sum = 0;
                for (auto i = 0; i < Nb; ++i)
                        sum += bit::intrinsic::popcount(data_[i]);
                return sum;
        }

        // representation

        std::array<Block, Nb> data_ = {{ 0 }};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
