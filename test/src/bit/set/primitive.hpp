#pragma once

namespace dctl {
namespace bit {

template<class T, int N = T::size()>
constexpr auto shift_left_assign(T const& b, int n)
{
        // arrange
        auto rhs = b;
        auto lhs = rhs;

        // act
        lhs <<= n;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                if (i < n)
                        check &= lhs.test(i) == 0;
                else
                        check &= lhs.test(i) == rhs.test(i - n);
        }
        return check;
}

template<class T, int N = T::size()>
constexpr auto shift_right_assign(T const& b, int n)
{
        // arrange
        auto rhs = b;
        auto lhs = rhs;

        // act
        lhs >>= n;

        // assert
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                if (i > N - 1 - n)
                        check &= lhs.test(i) == 0;
                else
                        check &= lhs.test(i) == rhs.test(i + n);
        }
        return check;
}

template<class T>
constexpr auto shift_left(T const& b, int n)
{
        // arrange
        auto rhs = b;
        auto lhs = rhs << n;

        // act
        rhs <<= n;

        // assert
        return lhs == rhs;
}

template<class T>
constexpr auto shift_right(T const& b, int n)
{
        // arrange
        auto rhs = b;
        auto lhs = rhs >> n;

        // act
        rhs >>= n;

        // assert
        return lhs == rhs;
}

}       // namespace bit
}       // namespace dctl
