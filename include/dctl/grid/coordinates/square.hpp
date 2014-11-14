#pragma once

namespace dctl {
namespace grid {
namespace ulo {

template<class Grid>
class Square
{
        int value_;
public:
        // constructors

        explicit constexpr Square(int v) noexcept
        :
                value_{v}
        {}

        // observers

        constexpr auto value () const noexcept
        {
                return value_;
        }
};

}       // namespace ulo
}       // namespace grid
}       // namespace dctl
