#pragma once
#include <cassert>                              // assert
#include <limits>                               // digits
#include <dctl/bit/detail/base_set_fwd.hpp>     // BaseSet
#include <dctl/bit/detail/intrinsic.hpp>        // popcount

namespace dctl {
namespace bit {
namespace detail {

template<class T, class Block>
struct BaseSet<T, Block, 1>
{
        static_assert(
                !std::numeric_limits<Block>::is_signed &&
                 std::numeric_limits<Block>::is_integer,
                "Block has to be of unsigned integer type."
        );

        enum { digits = std::numeric_limits<Block>::digits };
        enum { N = 1 * digits };

        Block& data()
        {
                return data_;
        }

        // structors

        constexpr BaseSet() noexcept = default;

        // element access

        constexpr auto block_ptr(T const& /* n */)
        {
                // assert(0 <= n && n <= N);
                return &data_;
        }

        constexpr auto block_ptr(T const& /* n */) const
        {
                // assert(0 <= n && n <= N);
                return &data_;
        }

        // bitwise operations

        void do_reset() noexcept
        {
                data_ = Block{0};
        }

        void do_set() noexcept
        {
                data_ = ~Block{0};
        }

        void do_flip() noexcept
        {
                data_ = ~data_;
        }

        void do_and(BaseSet const& other) noexcept
        {
                data_ &= other.data_;
        }

        void do_or(BaseSet const& other) noexcept
        {
                data_ |= other.data_;
        }

        void do_xor(BaseSet const& other) noexcept
        {
                data_ ^= other.data_;
        }

        void do_left_shift(int n)
        {
                assert(0 <= n && n < N);
                data_ <<= n;
        }

        void do_right_shift(int n)
        {
                assert(0 <= n && n < N);
                data_ >>= n;
        }

        // bitwise algorithms

        static auto do_equal(BaseSet const& lhs, BaseSet const& rhs) noexcept
        {
                return lhs.data_ == rhs.data_;
        }

        static auto do_lexicographical_compare(BaseSet const& lhs, BaseSet const& rhs) noexcept
        {
                return lhs.data_ < rhs.data_;
        }

        auto do_includes(BaseSet const& other) const noexcept
        {
                return (~data_ & other.data_) == Block{0};
        }

        auto do_intersects(BaseSet const& other) const noexcept
        {
                return (data_ & other.data_) != Block{0};
        }

        auto do_none() const noexcept
        {
                return data_ == Block{0};
        }

        auto do_any() const noexcept
        {
                return data_ != Block{0};
        }

        auto do_all() const noexcept
        {
                return data_ == ~Block{0};
        }

        auto do_count() const noexcept
        {
                return bit::intrinsic::popcount(data_);
        }

        template<class UnaryPredicate>
        auto do_count_until(UnaryPredicate /* pred */) const noexcept
        {
                return do_count();
        }

        // representation

        Block data_{};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
