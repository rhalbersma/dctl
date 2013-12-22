#pragma once
#include <stdexcept>                    // invalid_argument
#include <tuple>                        // tie

namespace dctl {
namespace grid {
namespace sco {

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


}       // namespace sco
}       // namespace grid
}       // namespace dctl
