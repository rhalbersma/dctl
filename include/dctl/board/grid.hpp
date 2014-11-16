#pragma once
#include <dctl/angle.hpp>               // Angle
#include <dctl/board/dimensions.hpp>    // Dimensions
#include <stdexcept>                    // invalid_argument

namespace dctl {
namespace board {

template<int>
class Grid;

// partial specialization definition
template<>
class Grid<0>
{
private:
        Dimensions dim;

public:
        explicit constexpr Grid(Dimensions const& d) noexcept : dim(d) {}

        constexpr auto width() const noexcept { return dim.width; }
        constexpr auto height() const noexcept { return dim.height; }
        constexpr auto inverted() const noexcept { return dim.inverted; }
        constexpr bool ll_parity() const noexcept { return board::ll_parity(dim); }
        constexpr bool ul_parity() const noexcept { return board::ul_parity(dim); }

        constexpr auto edge_columns() const noexcept { return 0; }

        // range of row pairs

        constexpr auto modulo() const noexcept { return width(); }

        // range of even (e) and odd (o) rows

        constexpr auto row_e() const noexcept { return (width() +  ul_parity()) / 2; }
        constexpr auto row_o() const noexcept { return (width() + !ul_parity()) / 2; }

        // left (l) and right (r) edges of even (e) and odd (o) rows

        constexpr auto edge_le() const noexcept { return 0; }
        constexpr auto edge_re() const noexcept { return edge_le() + (row_e() - 1); }
        constexpr auto edge_lo() const noexcept { return edge_re() + 1; }
        constexpr auto edge_ro() const noexcept { return edge_lo() + (row_o() - 1); }

        // grid size

        constexpr auto size() const noexcept { return (width() * height()) / 2 + (width() * height() * ul_parity()) % 2; }
};

template<int EdgeColumns>
class Grid
{
private:
        Grid<0> grid;

public:
        explicit constexpr Grid(Dimensions const& dim) noexcept : grid{dim} {}

        constexpr auto width    () const noexcept { return grid.width();     }
        constexpr auto height   () const noexcept { return grid.height();    }
        constexpr auto inverted () const noexcept { return grid.inverted();  }
        constexpr auto ll_parity() const noexcept { return grid.ll_parity(); }
        constexpr auto ul_parity() const noexcept { return grid.ul_parity(); }

        constexpr auto edge_columns() const noexcept { return EdgeColumns; }

        constexpr auto left_down () const noexcept { return (width() + edge_columns()) / 2; }
        constexpr auto right_down() const noexcept { return left_down() + 1;                }
        constexpr auto right     () const noexcept { return right_down() - left_down();     }
        constexpr auto down      () const noexcept { return right_down() + left_down();     }
        constexpr auto left_up   () const noexcept { return right_down();                   }
        constexpr auto right_up  () const noexcept { return left_down();                    }
        constexpr auto left      () const noexcept { return right();                        }
        constexpr auto up        () const noexcept { return down();                         }

        constexpr auto shift_size(Angle const& alpha) const
        {
                switch (alpha) {
                case   0: return right()     ;
                case  45: return right_up()  ;
                case  90: return up()        ;
                case 135: return left_up()   ;
                case 180: return left()      ;
                case 225: return left_down() ;
                case 270: return down()      ;
                case 315: return right_down();
                default: return throw std::invalid_argument("Shift direction angles shall be a multiple of 45 degrees"), 0;
                }
        }

        // range of row pairs

        constexpr auto modulo() const noexcept { return left_down() + right_down(); }

        // left (l) and right (r) edges of even (e) and odd (o) rows

        constexpr auto edge_le() const noexcept { return grid.edge_le(); }
        constexpr auto edge_re() const noexcept { return grid.edge_re(); }
        constexpr auto edge_lo() const noexcept { return left_down() + ul_parity(); }
        constexpr auto edge_ro() const noexcept { return edge_lo() + (grid.edge_ro() - grid.edge_lo()); }

        // grid size

        constexpr auto size() const noexcept { return modulo() * ((height() - 1) / 2) + ((height() % 2) ? edge_re() : edge_ro()) + 1; }
};

}       // namespace board
}       // namespace dctl
