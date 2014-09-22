#pragma once
#include <bit/set/primitive.hpp>

namespace dctl {
namespace bit {

template<class T, int N = T::size()>
constexpr auto and_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= and_assign(T{i}, T{j});
        return check;
}

template<class T, int N = T::size()>
constexpr auto or_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= or_assign(T{i}, T{j});
        return check;
}

template<class T, int N = T::size()>
constexpr auto xor_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= xor_assign(T{i}, T{j});
        return check;
}

template<class T, int N = T::size()>
constexpr auto minus_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= minus_assign(T{i}, T{j});
        return check;
}

template<class T, int N = T::size()>
constexpr auto shift_left_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= shift_left_assign(T{i}, j);
        return check;
}

template<class T, int N = T::size()>
constexpr auto shift_right_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= shift_right_assign(T{i}, j);
        return check;
}

template<class T, int N = T::size()>
constexpr auto count() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                check &= (~(~T{} << i)).count() == i;
        check &= (~T{}).count() == N;
        return check;
}

template<class T, int N = T::size()>
constexpr auto op_and() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= op_and(T{i}, T{j});
        return check;
}

template<class T, int N = T::size()>
constexpr auto op_or() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= op_or(T{i}, T{j});
        return check;
}

template<class T, int N = T::size()>
constexpr auto op_xor() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= op_xor(T{i}, T{j});
        return check;
}

template<class T, int N = T::size()>
constexpr auto op_minus() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= op_minus(T{i}, T{j});
        return check;
}

template<class T, int N = T::size()>
constexpr auto shift_left() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= shift_left(T{i}, j);
        return check;
}

template<class T, int N = T::size()>
constexpr auto shift_right() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i)
                for (auto j = 0; j < N; ++j)
                        check &= shift_right(T{i}, j);
        return check;
}

}       // namespace bit
}       // namespace dctl
