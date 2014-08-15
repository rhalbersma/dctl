#pragma once
#include <dctl/bit/detail/base_set_fwd.hpp>     // BaseSet
#include <dctl/bit/detail/intrinsic.hpp>        // popcount
#include <cassert>                              // assert
#include <cstddef>                              // size_t
#include <limits>                               // digits

namespace dctl {
namespace bit {
namespace detail {

template<class Key, class Compare, class Block>
struct BaseSet<Key, Compare, Block, 1>
{
        static_assert(
                !std::numeric_limits<Block>::is_signed &&
                 std::numeric_limits<Block>::is_integer,
                "Block has to be of unsigned integer type."
        );

        enum { digits = std::numeric_limits<Block>::digits };
        enum { N = 1 * digits };

        constexpr auto& data()
        {
                return data_;
        }

        constexpr auto const& data() const
        {
                return data_;
        }

        // constructors

        constexpr BaseSet() = default;

        // element access

        constexpr auto block_ptr(Key const& /* n */)
        {
                // assert(0 <= n && n <= N);
                return &data_;
        }

        constexpr auto block_ptr(Key const& /* n */) const
        {
                // assert(0 <= n && n <= N);
                return &data_;
        }

        // modifiers

        constexpr auto do_set() noexcept
        {
                data_ = ~Block{0};
        }

        constexpr auto do_reset() noexcept
        {
                data_ = Block{0};
        }

        constexpr auto do_flip() noexcept
        {
                data_ = ~data_;
        }

        constexpr auto do_and(BaseSet const& other) noexcept
        {
                data_ &= other.data_;
        }

        constexpr auto do_or(BaseSet const& other) noexcept
        {
                data_ |= other.data_;
        }

        constexpr auto do_xor(BaseSet const& other) noexcept
        {
                data_ ^= other.data_;
        }

        constexpr auto do_minus(BaseSet const& other) noexcept
        {
                data_ &= ~other.data_;
        }

        constexpr auto do_left_shift(std::size_t n)
        {
                assert(n < N);
                data_ <<= n;
        }

        constexpr auto do_right_shift(std::size_t n)
        {
                assert(n < N);
                data_ >>= n;
        }

        // queries

        constexpr auto do_count() const noexcept
        {
                return bit::intrinsic::popcount(data_);
        }

        template<class UnaryPredicate>
        constexpr auto do_count_until(UnaryPredicate /* pred */) const
        {
                return do_count();
        }

        // predicates

        constexpr auto do_all() const noexcept
        {
                return data_ == ~Block{0};
        }

        constexpr auto do_any() const noexcept
        {
                return data_ != Block{0};
        }

        constexpr auto do_none() const noexcept
        {
                return data_ == Block{0};
        }

        constexpr auto do_is_proper_subset_of(BaseSet const& other) const noexcept
        {
                if (data_ & ~other.data_)
                        return false;
                return (~data_ & other.data_) != Block{0};
        }

        constexpr auto do_is_subset_of(BaseSet const& other) const noexcept
        {
                return (data_ & ~other.data_) == Block{0};
        }

        constexpr auto do_intersects(BaseSet const& other) const noexcept
        {
                return (data_ & other.data_) != Block{0};
        }

        constexpr auto do_equal(BaseSet const& other) const noexcept
        {
                return data_ == other.data_;
        }

        constexpr auto do_colexicographical_compare(BaseSet const& other) const noexcept
        {
                return Compare{}(data_, other.data_);
        }

        // representation

        Block data_{};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
