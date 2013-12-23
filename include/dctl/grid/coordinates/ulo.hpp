#pragma once
#include <dctl/grid/coordinates/detail/coordinates.hpp> // Coordinates

namespace dctl {
namespace grid {
namespace ulo {

template<class Grid>
class Coordinates
:
        public detail::Coordinates
{
public:
        // structors

        using detail::Coordinates::Coordinates;
};

}       // namespace ulo
}       // namespace grid
}       // namespace dctl
