#pragma once
#include <dctl/grid/coordinates/detail/coordinates.hpp> // Coordinates

namespace dctl {
namespace grid {
namespace ulo {

template<class Grid>
class Square
{
public:
        using grid_type = Grid;

        explicit constexpr Square(int v) noexcept
        :
                value_{v}
        {}

        constexpr auto value () const noexcept
        {
                return value_;
        }

private:
        int value_;
};

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
