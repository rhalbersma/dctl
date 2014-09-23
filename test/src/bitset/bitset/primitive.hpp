#pragma once
#include <dctl/bitset.hpp>      // bitset

namespace xstd {

// comparators

template<int N>
constexpr auto equal(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        // arrange
        auto prev = true;
        for (auto i = 0; i < N; ++i)
                prev &= lhs.test(i) == rhs.test(i);

        // act
        auto const next = lhs == rhs;

        // assert
        return next == prev;
}

template<int N>
constexpr auto not_equal(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        // arrange
        auto const prev = !(lhs == rhs);

        // act
        auto const next = lhs != rhs;

        // assert
        return next == prev;
}

// modifiers

template<int N>
constexpr auto and_assign(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        // arrange
        auto next = lhs;
        auto const prev = lhs;

        // act
        next &= rhs;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i)
                if (!rhs.test(i))
                        check &= !next.test(i);
                else
                        check &= next.test(i) == prev.test(i);
        return check;
}

template<int N>
constexpr auto or_assign(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        // arrange
        auto next = lhs;
        auto const prev = lhs;

        // act
        next |= rhs;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i)
                if (rhs.test(i))
                        check &= next.test(i);
                else
                        check &= next.test(i) == prev.test(i);
        return check;
}

template<int N>
constexpr auto xor_assign(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        // arrange
        auto next = lhs;
        auto const prev = lhs;

        // act
        next ^= rhs;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i)
                if (rhs.test(i))
                        check &= next.test(i) == !prev.test(i);
                else
                        check &= next.test(i) == prev.test(i);
        return check;
}

template<int N>
constexpr auto minus_assign(bitset<N> const& lhs, bitset<N> const& rhs) noexcept
{
        // arrange
        auto next = lhs;
        auto const prev = lhs;

        // act
        next -= rhs;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i)
                if (rhs.test(i))
                        check &= !next.test(i);
                else
                        check &= next.test(i) == prev.test(i);
        return check;
}

template<int N>
constexpr auto shift_left_assign(bitset<N> const& b, int n)
{
        // arrange
        auto next = b;
        auto const prev = b;

        // act
        next <<= n;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i)
                if (i < n)
                        check &= !next.test(i);
                else
                        check &= next.test(i) == prev.test(i - n);
        return check;
}

template<int N>
constexpr auto shift_right_assign(bitset<N> const& b, int n)
{
        // arrange
        auto next = b;
        auto const prev = b;

        // act
        next >>= n;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i)
                if (i > N - 1 - n)
                        check &= !next.test(i);
                else
                        check &= next.test(i) == prev.test(i + n);
        return check;
}

// observers

template<int N>
constexpr auto count(bitset<N> const& b) noexcept
{
        // arrange
        auto prev = 0;
        for (auto i = 0; i < N; ++i)
                prev += b.test(i);

        // act
        auto const next = b.count();
        
        // assert
        return next == prev;
}

// operators

template<int N>
constexpr auto op_and(bitset<N> const& lhs, bitset<N> const& rhs)
{
        // arrange
        auto prev = lhs;
        prev &= rhs;

        // act
        auto const next = lhs & rhs;

        // assert
        return next == prev;
}

template<int N>
constexpr auto op_or(bitset<N> const& lhs, bitset<N> const& rhs)
{
        // arrange
        auto prev = lhs;
        prev |= rhs;

        // act
        auto const next = lhs | rhs;

        // assert
        return next == prev;
}

template<int N>
constexpr auto op_xor(bitset<N> const& lhs, bitset<N> const& rhs)
{
        // arrange
        auto prev = lhs;
        prev ^= rhs;

        // act
        auto const next = lhs ^ rhs;

        // assert
        return next == prev;
}

template<int N>
constexpr auto op_minus(bitset<N> const& lhs, bitset<N> const& rhs)
{
        // arrange
        auto prev = lhs;
        prev -= rhs;

        // act
        auto const next = lhs - rhs;

        // assert
        return next == prev;
}

template<int N>
constexpr auto op_shift_left(bitset<N> const& b, int n)
{
        // arrange
        auto prev = b;
        prev <<= n;

        // act
        auto const next = b << n;

        // assert
        return next == prev;
}

template<int N>
constexpr auto op_shift_right(bitset<N> const& b, int n)
{
        // arrange
        auto prev = b;
        prev >>= n;

        // act
        auto const next = b >> n;

        // assert
        return next == prev;
}

}       // namespace xstd
