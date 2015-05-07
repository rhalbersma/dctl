#pragma once
#include <dctl/angle.hpp>       // Angle
#include <stdexcept>            // invalid_argument
#include <tuple>                // forward_as_tuple

namespace dctl {
namespace board {

struct Dimensions
{
        int width, height;
        bool inverted;
};

constexpr auto ll_parity(Dimensions const& dim) noexcept
{
        return !dim.inverted;
}

constexpr auto ul_parity(Dimensions const& dim) noexcept
{
        return static_cast<bool>(dim.inverted ^ (dim.height % 2));
}

inline
constexpr auto
operator==(Dimensions const& lhs, Dimensions const& rhs) noexcept
{
        return
               lhs.width == rhs.width && lhs.height == rhs.height && lhs.inverted == rhs.inverted
               //std::forward_as_tuple(lhs.width, lhs.height, lhs.inverted) ==
               //std::forward_as_tuple(rhs.width, rhs.height, rhs.inverted)
        ;
}

inline
constexpr auto
operator!=(Dimensions const& lhs, Dimensions const& rhs) noexcept
{
        return !(lhs == rhs);
}

inline
constexpr auto rotate(Dimensions const& dim, Angle const& theta)
{
        switch (theta) {
        case   0 : return dim;
        case  90 : return Dimensions{ dim.height, dim.width , static_cast<bool>((dim.width % 2) ^                    !dim.inverted) };
        case 180 : return Dimensions{ dim.width , dim.height, static_cast<bool>((dim.width % 2) ^ (dim.height % 2) ^  dim.inverted) };
        case 270 : return Dimensions{ dim.height, dim.width , static_cast<bool>(                  (dim.height % 2) ^ !dim.inverted) };
        default  : return throw std::invalid_argument("Dimensions rotation angles shall be a multiple of 90 degrees."), dim;
        }
}

}       // namespace board
}       // namespace dctl
