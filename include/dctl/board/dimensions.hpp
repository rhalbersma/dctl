#pragma once
#include <dctl/board/angle.hpp> // Angle
#include <cassert>              // assert
#include <stdexcept>            // invalid_argument
#include <tuple>                // forward_as_tuple

namespace dctl {
namespace board {

class Dimensions
{
        int width_, height_;
        bool inverted_;
public:
        constexpr Dimensions(int w, int h, bool i) noexcept
        :
                width_{w}, height_{h}, inverted_{i}
        {
                assert(0 <= width_ );
                assert(0 <= height_);
        }

        constexpr auto width() const noexcept { return width_; }
        constexpr auto height() const noexcept { return height_; }
        constexpr auto inverted() const noexcept { return inverted_; }
};

constexpr auto operator==(Dimensions const& lhs, Dimensions const& rhs) noexcept
{
        return
               std::forward_as_tuple(lhs.width(), lhs.height(), lhs.inverted()) ==
               std::forward_as_tuple(rhs.width(), rhs.height(), rhs.inverted())
        ;
}

constexpr auto operator!=(Dimensions const& lhs, Dimensions const& rhs) noexcept
{
        return !(lhs == rhs);
}

constexpr auto width_parity(Dimensions const& dim) noexcept
{
        return static_cast<bool>(dim.width() % 2);
}

constexpr auto height_parity(Dimensions const& dim) noexcept
{
        return static_cast<bool>(dim.height() % 2);
}

constexpr auto ll_parity(Dimensions const& dim) noexcept
{
        return !dim.inverted();
}

constexpr auto ul_parity(Dimensions const& dim) noexcept
{
        return static_cast<bool>(height_parity(dim) ^ !ll_parity(dim));
}

constexpr auto rotate(Dimensions const& dim, Angle const& a)
{
        switch (a) {
        case   0 : return dim;
        case  90 : return Dimensions{ dim.height(), dim.width() , static_cast<bool>(width_parity(dim)  ^ ll_parity(dim)) };
        case 180 : return Dimensions{ dim.width() , dim.height(), static_cast<bool>(width_parity(dim)  ^ ul_parity(dim)) };
        case 270 : return Dimensions{ dim.height(), dim.width() , static_cast<bool>(height_parity(dim) ^ ll_parity(dim)) };
        default  : return throw std::invalid_argument("Rotations of Dimensions objects shall be in multiples of 90 degrees."), dim;
        }
}

}       // namespace board
}       // namespace dctl
