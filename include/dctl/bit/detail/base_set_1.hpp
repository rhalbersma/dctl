#pragma once
#include <dctl/bit/detail/base_set_fwd.hpp>     // base_set
#include <dctl/bit/intrinsic.hpp>               // popcount

namespace dctl {
namespace bit {
namespace detail {

template<class T, class Block>
struct base_set<T, Block, 1>
{
        // structors

        constexpr base_set() noexcept = default;

        explicit constexpr base_set(Block block) noexcept
        :
                data_{block}
        {}

        // element access

        constexpr auto block_ptr(T /* n */) noexcept
        {
                return &data_;
        }

        constexpr auto block_ptr(T /* n */) const noexcept
        {
                return &data_;
        }

        static constexpr auto index(T n) noexcept
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

        constexpr void do_left_shift(int n)
        {
                data_ <<= n;
        }

        constexpr void do_right_shift(int n)
        {
                data_ >>= n;
        }

        // bitwise algorithms

        static constexpr auto do_equal(base_set const& lhs, base_set const& rhs) noexcept
        {
                return lhs.data_ == rhs.data_;
        }

        static constexpr auto do_lexicographical_compare(base_set const& lhs, base_set const& rhs) noexcept
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

        constexpr auto do_count_equal_to(int n) const noexcept
        {
                return do_count() == n;
        }

        constexpr auto do_count_less(int n) const noexcept
        {
                return do_count() < n;
        }

        constexpr auto do_count() const noexcept
        {
                return intrinsic::popcount(data_);
        }

        // representation

        Block data_ {};
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
