#pragma once

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

private:
        // representation

        int row_;
        int col_;
};

}       // namespace detail
}       // namespace grid
}       // namespace dctl
