#pragma once
#include <dctl/bit/detail/base_set_fwd.hpp>     // base_set

namespace dctl {
namespace bit {
namespace detail {

template<class WordT>
struct base_set<WordT, 1>
{
        // structors

        constexpr base_set() noexcept = default;

        // pointers

        constexpr auto begin_ptr() noexcept
        {
                return &data_;
        }

        constexpr auto begin_ptr() const noexcept
        {
                return &data_;
        }

        constexpr auto end_ptr() noexcept
        {
                return &data_;
        }

        constexpr auto end_ptr() const noexcept
        {
                return &data_;
        }

        constexpr auto word_ptr(int /* n */) noexcept
        {
                return &data_;
        }

        constexpr auto word_ptr(int /* n */) const noexcept
        {
                return &data_;
        }

        constexpr auto word_offset(int n) noexcept
        {
                return n;
        }

        constexpr auto word_offset(int n) const noexcept
        {
                return n;
        }

        // capacity

        constexpr auto do_empty() const noexcept
        {
                return data_ == 0;
        }

        constexpr auto do_size() const noexcept
        {
                return bit::intrinsic::size(data_);
        }

        // modifiers

        constexpr void do_clear() noexcept
        {
                data_ = 0;
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

        // representation

        WordT data_ = { 0 };
};

}       // namespace detail
}       // namespace bit
}       // namespace dctl
