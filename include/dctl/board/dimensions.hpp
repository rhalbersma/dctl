#pragma once
#include <dctl/board/angle.hpp> // angle
#include <cassert>              // assert
#include <stdexcept>            // invalid_argument
#include <tuple>                // tie

namespace dctl {
namespace board {

class Dimensions
{
        int const width_;
        int const height_;
        bool const inverted_;

        constexpr auto assert_invariant() const noexcept
        {
                assert(0 <= width_ );
                assert(0 <= height_);
        }

public:
        constexpr Dimensions(int const w, int const h, bool const i) noexcept
        :
                width_{w}, height_{h}, inverted_{i}
        {
                assert_invariant();
        }

        constexpr auto width() const noexcept { return width_; }
        constexpr auto height() const noexcept { return height_; }
        constexpr auto inverted() const noexcept { return inverted_; }

        constexpr auto tied() const noexcept { return std::tie(width_, height_, inverted_); }
};

constexpr auto operator==(Dimensions const& lhs, Dimensions const& rhs) noexcept
{
        return lhs.tied() == rhs.tied();
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

constexpr auto rotate(Dimensions const& dim, angle const a)
{
        switch (a.degrees()) {
        case   0 : return dim;
        case  90 : return Dimensions{ dim.height(), dim.width() , static_cast<bool>(width_parity(dim)  ^ ll_parity(dim)) };
        case 180 : return Dimensions{ dim.width() , dim.height(), static_cast<bool>(width_parity(dim)  ^ ul_parity(dim)) };
        case 270 : return Dimensions{ dim.height(), dim.width() , static_cast<bool>(height_parity(dim) ^ ll_parity(dim)) };
        default  : return static_cast<void>(throw std::invalid_argument("Rotations of Dimensions objects shall be in multiples of 90 degrees.")), dim;
        }
}

}       // namespace board
}       // namespace dctl
