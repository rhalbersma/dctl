#pragma once
#include <dctl/bit/detail/base_set_fwd.hpp>     // base_set

namespace dctl {
namespace bit {
namespace detail {

template<class T, class WordT>
struct base_set<T, WordT, 1>
{
        // structors

        constexpr base_set() noexcept = default;

        // element access

        constexpr auto segment_ptr(T /* n */) noexcept
        {
                return &data_;
        }

        constexpr auto segment_ptr(T /* n */) const noexcept
        {
                return &data_;
        }

        constexpr auto& segment(T /* n */) noexcept
        {
                return data_;
        }

        constexpr auto const& segment(T /* n */) const noexcept
        {
                return data_;
        }

        static constexpr auto index(T n) noexcept
        {
                return n;
        }

        // bitwise operations

        constexpr void do_reset() noexcept
        {
                data_ = 0;
        }

        constexpr void do_set() noexcept
        {
                data_ = ~0;
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

        constexpr void do_left_shift(std::size_t n)
        {
                data_ <<= n;
        }

        constexpr void do_right_shift(std::size_t n)
        {
                data_ >>= n;
        }

        // bitwise algorithms

        constexpr bool do_equal(base_set const& other) const noexcept
        {
                return data_ == other.data_;
        }

        constexpr bool do_lexicographical_compare(base_set const& other) const noexcept
        {
                return data_ < other.data_;
        }

        constexpr auto do_none() const noexcept
        {
                return data_ == 0;
        }

        constexpr auto do_any() const noexcept
        {
                return data_ != 0;
        }

        constexpr auto do_all() const noexcept
        {
                return data_ == ~0;
        }

        constexpr auto do_count() const noexcept
        {
                return bit::intrinsic::popcount(data_);
        }

        // representation

        WordT data_ = { 0 };
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
