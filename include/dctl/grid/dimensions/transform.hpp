#pragma once
#include <stdexcept>                    // logic_error
#include <dctl/angle/angle.hpp>         // make_angle
#include <dctl/angle/degrees.hpp>       // D000, L090, R090, D180
#include <dctl/grid/dimensions.hpp>     // Dimensions

namespace dctl {
namespace grid {
namespace detail {

template<class T>
constexpr auto rotate(Dimensions const& dim, T const& theta)
{
        switch(make_angle(theta)) {
        case angle::D000: return dim;
        case angle::L090: return Dimensions{ dim.width() , dim.height(), static_cast<bool>((dim.height() % 2) ^ !dim.parity()) };
        case angle::R090: return Dimensions{ dim.width() , dim.height(), static_cast<bool>((dim.width()  % 2) ^ !dim.parity()) };
        case angle::D180: return Dimensions{ dim.height(), dim.width() , static_cast<bool>((dim.height() % 2) ^ (dim.width() % 2) ^ (!!dim.parity())) };
        default: return throw std::logic_error("Dimensions rotation angles shall be a multiple of 90 degrees."), dim;
        }
}

}       // namespace detail
}       // namespace grid
}       // namespace dctl
