#pragma once

namespace dctl {
namespace bit {

template<class T, int N = T::size()>
constexpr auto and_assign(T const& lhs, T const& rhs) noexcept
{
        // arrange
        auto next = lhs;
        auto const prev = lhs;

        // act
        next &= rhs;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                if (!rhs.test(i))
                        check &= !next.test(i);
                else
                        check &= next.test(i) == prev.test(i);
        }
        return check;
}

template<class T, int N = T::size()>
constexpr auto or_assign(T const& lhs, T const& rhs) noexcept
{
        // arrange
        auto next = lhs;
        auto const prev = lhs;

        // act
        next |= rhs;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                if (rhs.test(i))
                        check &= next.test(i);
                else
                        check &= next.test(i) == prev.test(i);
        }
        return check;
}

template<class T, int N = T::size()>
constexpr auto xor_assign(T const& lhs, T const& rhs) noexcept
{
        // arrange
        auto next = lhs;
        auto const prev = lhs;

        // act
        next ^= rhs;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                if (rhs.test(i))
                        check &= next.test(i) == !prev.test(i);
                else
                        check &= next.test(i) == prev.test(i);
        }
        return check;
}

template<class T, int N = T::size()>
constexpr auto minus_assign(T const& lhs, T const& rhs) noexcept
{
        // arrange
        auto next = lhs;
        auto const prev = lhs;

        // act
        next -= rhs;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                if (rhs.test(i))
                        check &= !next.test(i);
                else
                        check &= next.test(i) == prev.test(i);
        }
        return check;
}

template<class T, int N = T::size()>
constexpr auto shift_left_assign(T const& b, int n)
{
        // arrange
        auto next = b;
        auto const prev = b;

        // act
        next <<= n;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                if (i < n)
                        check &= !next.test(i);
                else
                        check &= next.test(i) == prev.test(i - n);
        }
        return check;
}

template<class T, int N = T::size()>
constexpr auto shift_right_assign(T const& b, int n)
{
        // arrange
        auto next = b;
        auto const prev = b;

        // act
        next >>= n;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                if (i > N - 1 - n)
                        check &= !next.test(i);
                else
                        check &= next.test(i) == prev.test(i + n);
        }
        return check;
}

template<class T, int N = T::size()>
constexpr auto count(T const& b) noexcept
{
        // arrange
        auto result = 0;

        // act
        for (auto i = 0; i < N; ++i)
                result += b.test(i);

        // assert
        return b.count() == result;
}

template<class T>
constexpr auto op_and(T const& lhs, T const& rhs)
{
        // arrange
        auto next = lhs;

        // act
        next &= rhs;

        // assert
        return (lhs & rhs) == next;
}

template<class T>
constexpr auto op_or(T const& lhs, T const& rhs)
{
        // arrange
        auto next = lhs;

        // act
        next |= rhs;

        // assert
        return (lhs | rhs) == next;
}

template<class T>
constexpr auto op_xor(T const& lhs, T const& rhs)
{
        // arrange
        auto next = lhs;

        // act
        next ^= rhs;

        // assert
        return (lhs ^ rhs) == next;
}

template<class T>
constexpr auto op_minus(T const& lhs, T const& rhs)
{
        // arrange
        auto next = lhs;

        // act
        next -= rhs;

        // assert
        return (lhs - rhs) == next;
}

template<class T>
constexpr auto shift_left(T const& b, int n)
{
        // arrange
        auto next = b;

        // act
        next <<= n;

        // assert
        return (b << n) == next;
}

template<class T>
constexpr auto shift_right(T const& b, int n)
{
        // arrange
        auto next = b;

        // act
        next >>= n;

        // assert
        return (b >> n) == next;
}

}       // namespace bit
}       // namespace dctl
