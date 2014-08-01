#pragma once

namespace dctl {
namespace grid {
namespace ulo {

template<class Grid>
class Square
{
public:
        // nested types

        using grid_type = Grid;

        // constructors

        explicit constexpr Square(int v) noexcept
        :
                value_{v}
        {}

        // queries

        constexpr auto value () const noexcept
        {
                return value_;
        }

private:
        // representation

        int value_;
};

}       // namespace ulo
}       // namespace grid
}       // namespace dctl
