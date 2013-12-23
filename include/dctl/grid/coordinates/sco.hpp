#pragma once
#include <stdexcept>                                    // invalid_argument
#include <dctl/grid/coordinates/detail/coordinates.hpp> // Coordinates

namespace dctl {
namespace grid {
namespace sco {

class Coordinates
:
        public detail::Coordinates
{
public:
        // structors

        using detail::Coordinates::Coordinates;
};

}       // namespace sco
}       // namespace grid
}       // namespace dctl
