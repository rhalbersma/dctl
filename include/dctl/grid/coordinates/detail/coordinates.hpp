#pragma once
#include <tuple>                        // forward_as_tuple

namespace dctl {
namespace grid {
namespace detail {

class Coordinates
{
public:
        // structors

        constexpr Coordinates(int r, int c) noexcept
        :
                row_{r},
                col_{c}
        {}

        // queries

        constexpr int row() const noexcept
        {
                return row_;
        }

        constexpr int col() const noexcept
        {
                return col_;
        }

        // predicates

        friend /* constexpr */ bool
        operator==(Coordinates const& lhs, Coordinates const& rhs) noexcept
        {
                return
                        std::forward_as_tuple(lhs.row_, lhs.col_) ==
                        std::forward_as_tuple(rhs.row_, rhs.col_)
                ;
        }

        friend /* constexpr */ bool
        operator!=(Coordinates const& lhs, Coordinates const& rhs) noexcept
        {
                return !(lhs == rhs);
        }

private:
        // representation

        int row_;
        int col_;
};

}       // namespace detail
}       // namespace grid
}       // namespace dctl
