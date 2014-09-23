#pragma once
#include <bitset/bitset/primitive.hpp>

namespace xstd {

// comparators

template<int N>
constexpr auto equal() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= equal(bitset<N>{i}, bitset<N>{j});
        return check;
}

template<int N>
constexpr auto not_equal() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= not_equal(bitset<N>{i}, bitset<N>{j});
        return check;
}

// modifiers

template<int N>
constexpr auto and_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= and_assign(bitset<N>{i}, bitset<N>{j});
        return check;
}

template<int N>
constexpr auto or_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= or_assign(bitset<N>{i}, bitset<N>{j});
        return check;
}

template<int N>
constexpr auto xor_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= xor_assign(bitset<N>{i}, bitset<N>{j});
        return check;
}

template<int N>
constexpr auto minus_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= minus_assign(bitset<N>{i}, bitset<N>{j});
        return check;
}

template<int N>
constexpr auto shift_left_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= shift_left_assign(bitset<N>{i}, j);
        return check;
}

template<int N>
constexpr auto shift_right_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= shift_right_assign(bitset<N>{i}, j);
        return check;
}

// observers

template<int N>
constexpr auto count() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                check &= (~(~bitset<N>{} << i)).count() == i;
        if (N)
                check &= bitset<N>{}.flip().count() == N;
        else
                check &= bitset<N>{}.flip().count() == 0;
        return check;
}

// operators

template<int N>
constexpr auto op_and() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= op_and(bitset<N>{i}, bitset<N>{j});
        return check;
}

template<int N>
constexpr auto op_or() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= op_or(bitset<N>{i}, bitset<N>{j});
        return check;
}

template<int N>
constexpr auto op_xor() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= op_xor(bitset<N>{i}, bitset<N>{j});
        return check;
}

template<int N>
constexpr auto op_minus() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= op_minus(bitset<N>{i}, bitset<N>{j});
        return check;
}

template<int N>
constexpr auto op_shift_left() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= op_shift_left(bitset<N>{i}, j);
        return check;
}

template<int N>
constexpr auto op_shift_right() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= op_shift_right(bitset<N>{i}, j);
        return check;
}

}       // namespace xstd
