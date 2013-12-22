#pragma once
#include <stdexcept>                                    // invalid_argument
#include <tuple>                                        // forward_as_tuple
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

        // predicates

        friend /* constexpr */ bool
        operator==(Coordinates const& lhs, Coordinates const& rhs) noexcept
        {
                return
                        std::forward_as_tuple(lhs.row(), lhs.col()) ==
                        std::forward_as_tuple(rhs.row(), rhs.col())
                ;
        }

        friend /* constexpr */ bool
        operator!=(Coordinates const& lhs, Coordinates const& rhs) noexcept
        {
                return !(lhs == rhs);
        }
};

}       // namespace sco
}       // namespace grid
}       // namespace dctl
