#pragma once
#include <dctl/board/angle.hpp> // angle
#include <cstddef>              // size_t
#include <stdexcept>            // invalid_argument
#include <tuple>                // tie

namespace dctl {
namespace board {
namespace detail {

struct dimensions
{
        std::size_t const width;
        std::size_t const height;
        bool const is_inverted;
};

constexpr auto tied(dimensions const dim) noexcept
{
        return std::tie(dim.width, dim.height, dim.is_inverted);
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

constexpr auto rotate(dimensions const dim, angle const a)
        -> dimensions
{
        switch (a.degrees()) {
        case   0 : return dim;
        case  90 : return { dim.height, dim.width , !upper_left_is_square(dim) };
        case 180 : return { dim.width , dim.height, !upper_right_is_square(dim) };
        case 270 : return { dim.height, dim.width , !lower_right_is_square(dim) };
        default  : return static_cast<void>(throw std::invalid_argument("Not a multiple of 90 degrees.")), dim;
        }
}

}       // namespace detail
}       // namespace board
}       // namespace dctl
