#pragma once
#include <cassert>                              // assert
#include <limits>                               // digits
#include <dctl/bit/detail/base_set_fwd.hpp>     // base_set
#include <dctl/bit/intrinsic.hpp>               // popcount

namespace dctl {
namespace bit {
namespace detail {

template<class T, class Block>
struct base_set<T, Block, 1>
{
        static_assert(
                !std::numeric_limits<Block>::is_signed &&
                 std::numeric_limits<Block>::is_integer,
                 "Block has to be of unsigned integer type."
        );

        // structors

        constexpr base_set() noexcept = default;

        explicit constexpr base_set(Block block) noexcept
        :
                data_{block}
        {}

        // element access

        constexpr auto block_ptr(T const& /* n */) noexcept
        {
                return &data_;
        }

        constexpr auto block_ptr(T const& /* n */) const noexcept
        {
                return &data_;
        }

        static constexpr auto index(T const& n) noexcept
        {
                return n;
        }

        // bitwise operations

        constexpr void do_reset() noexcept
        {
                data_ = Block{0};
        }

        constexpr void do_set() noexcept
        {
                data_ = ~Block{0};
        }

        constexpr void do_flip() noexcept
        {
                data_ = ~data_;
        }

        constexpr void do_and(base_set const& other) noexcept
        {
                data_ &= other.data_;
        }

        constexpr void do_or(base_set const& other) noexcept
        {
                data_ |= other.data_;
        }

        constexpr void do_xor(base_set const& other) noexcept
        {
                data_ ^= other.data_;
        }

        constexpr void do_left_shift(int n) noexcept
        {
                assert(0 <= n < std::numeric_limits<Block>::digits);
                data_ <<= n;
        }

        constexpr void do_right_shift(int n) noexcept
        {
                assert(0 <= n < std::numeric_limits<Block>::digits);
                data_ >>= n;
        }

        // bitwise algorithms

        static constexpr auto
        do_equal(base_set const& lhs, base_set const& rhs) noexcept
        {
                return lhs.data_ == rhs.data_;
        }

        static constexpr auto
        do_lexicographical_compare(base_set const& lhs, base_set const& rhs) noexcept
        {
                return lhs.data_ < rhs.data_;
        }

        constexpr auto do_includes(base_set const& other) const noexcept
        {
                return (other.data_ & ~data_) == Block{0};
        }

        constexpr auto do_intersects(base_set const& other) const noexcept
        {
                return (data_ & other.data_) != Block{0};
        }

        constexpr auto do_none() const noexcept
        {
                return data_ == Block{0};
        }

        constexpr auto do_any() const noexcept
        {
                return data_ != Block{0};
        }

        constexpr auto do_all() const noexcept
        {
                return data_ == ~Block{0};
        }

        constexpr auto do_is_count_equal_to(int n) const noexcept
        {
                return bit::popcount(data_) == n;
        }

        constexpr auto do_is_count_less(int n) const noexcept
        {
                return bit::popcount(data_) < n;
        }

        constexpr auto do_count() const noexcept
        {
                return bit::popcount(data_);
        }

        // representation

        Block data_{};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
