#pragma once
#include <bit/set/primitive.hpp>

namespace dctl {
namespace bit {

template<class T, int N = T::size()>
constexpr auto shift_left_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                auto const b = T{i};
                for (auto n = 0; n < N; ++n)
                        check &= shift_left_assign(b, n);
        }
        return check;
}

template<class T, int N = T::size()>
constexpr auto shift_right_assign() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                auto const b = T{i};
                for (auto n = 0; n < N; ++n)
                        check &= shift_right_assign(b, n);
        }
        return check;
}

template<class T, int N = T::size()>
constexpr auto shift_left() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                auto const b = T{i};
                for (auto n = 0; n < N; ++n)
                        check &= shift_left(b, n);
        }
        return check;
}

template<class T, int N = T::size()>
constexpr auto shift_right() noexcept
{
        auto check = true;
        for (auto i = 0; i < N; ++i) {
                auto const b = T{i};
                for (auto n = 0; n < N; ++n)
                        check &= shift_right(b, n);
        }
        return check;
}

}       // namespace bit
}       // namespace dctl
