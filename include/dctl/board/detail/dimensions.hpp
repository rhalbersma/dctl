#pragma once
#include <dctl/board/angle.hpp> // angle
#include <cstddef>              // size_t
#include <stdexcept>            // invalid_argument
#include <tuple>                // tie

namespace dctl {
namespace board {
namespace detail {

struct Dimensions
{
        std::size_t const width;
        std::size_t const height;
        bool const is_inverted;
};

constexpr auto tied(Dimensions const dim) noexcept
{
        return std::tie(dim.width, dim.height, dim.is_inverted);
}

constexpr auto operator==(Dimensions const lhs, Dimensions const rhs) noexcept
{
        return tied(lhs) == tied(rhs);
}

constexpr auto operator!=(Dimensions const lhs, Dimensions const rhs) noexcept
{
        return !(lhs == rhs);
}

constexpr auto width_parity(Dimensions const dim) noexcept
{
        return static_cast<bool>(dim.width % 2);
}

constexpr auto height_parity(Dimensions const dim) noexcept
{
        return static_cast<bool>(dim.height % 2);
}

constexpr auto lower_left_is_square(Dimensions const dim) noexcept
{
        return !dim.is_inverted;
}

constexpr auto upper_left_is_square(Dimensions const dim) noexcept
{
        return height_parity(dim) ^ !lower_left_is_square(dim);
}

constexpr auto upper_right_is_square(Dimensions const dim) noexcept
{
        return width_parity(dim) ^ height_parity(dim) ^ lower_left_is_square(dim);
}

constexpr auto lower_right_is_square(Dimensions const dim) noexcept
{
        return width_parity(dim) ^ !lower_left_is_square(dim);
}

constexpr auto rotate(Dimensions const dim, angle const a)
{
        switch (a.degrees()) {
        case   0 : return dim;
        case  90 : return Dimensions{ dim.height, dim.width , !upper_left_is_square(dim) };
        case 180 : return Dimensions{ dim.width , dim.height, !upper_right_is_square(dim) };
        case 270 : return Dimensions{ dim.height, dim.width , !lower_right_is_square(dim) };
        default  : return static_cast<void>(throw std::invalid_argument("Rotations of Dimensions objects shall be in multiples of 90 degrees.")), dim;
        }
}

}       // namespace detail
}       // namespace board
}       // namespace dctl
