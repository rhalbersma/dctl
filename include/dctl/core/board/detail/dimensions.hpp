#pragma once
#include <dctl/core/board/angle.hpp> // angle
#include <cassert>              // assert
#include <tuple>                // tie

namespace dctl {
namespace board {
namespace detail {

struct dimensions
{
        int const width;
        int const height;
        bool const is_inverted;
};

constexpr auto tied(dimensions const dim) noexcept
{
        return std::make_tuple(dim.width, dim.height, dim.is_inverted);
}

constexpr auto operator==(dimensions const lhs, dimensions const rhs) noexcept
{
        return tied(lhs) == tied(rhs);
}

constexpr auto operator!=(dimensions const lhs, dimensions const rhs) noexcept
{
        return !(lhs == rhs);
}

constexpr auto width_parity(dimensions const dim) noexcept
{
        return static_cast<bool>(dim.width % 2);
}

constexpr auto height_parity(dimensions const dim) noexcept
{
        return static_cast<bool>(dim.height % 2);
}

constexpr auto lower_left_is_square(dimensions const dim) noexcept
{
        return !dim.is_inverted;
}

constexpr auto upper_left_is_square(dimensions const dim) noexcept
{
        return height_parity(dim) ^ !lower_left_is_square(dim);
}

constexpr auto upper_right_is_square(dimensions const dim) noexcept
{
        return width_parity(dim) ^ height_parity(dim) ^ lower_left_is_square(dim);
}

constexpr auto lower_right_is_square(dimensions const dim) noexcept
{
        return width_parity(dim) ^ !lower_left_is_square(dim);
}

constexpr auto rotate(dimensions const dim, angle const a) noexcept
        -> dimensions
{
        switch (a.value()) {
        case   0 : return dim;
        case  90 : return { dim.height, dim.width , !upper_left_is_square(dim) };
        case 180 : return { dim.width , dim.height, !upper_right_is_square(dim) };
        case 270 : return { dim.height, dim.width , !lower_right_is_square(dim) };
        default  : assert(false); return dim;
        }
}

}       // namespace detail
}       // namespace board
}       // namespace dctl
