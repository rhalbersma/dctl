#pragma once
#include <tuple>                        // tie

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
{
public:
        constexpr Coordinates(int r, int c) noexcept
        :
                row_{r},
                col_{c}
        {}

        constexpr auto row() const noexcept
        {
                return row_;
        }

        constexpr auto col() const noexcept
        {
                return col_;
        }

        friend /* constexpr */ auto
        operator==(Coordinates const& lhs, Coordinates const& rhs) noexcept
        {
                return
                        std::tie(lhs.row_, rhs.col_) ==
                        std::tie(rhs.row_, rhs.col_)
                ;
        }

        friend /* constexpr */ auto
        operator!=(Coordinates const& lhs, Coordinates const& rhs) noexcept
        {
                return !(lhs == rhs);
        }

private:
        int row_;
        int col_;
};

}       // namespace ulo
}       // namespace grid
}       // namespace dctl
