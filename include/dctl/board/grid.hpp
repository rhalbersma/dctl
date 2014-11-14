#pragma once
#include <dctl/board/grid_fwd.hpp>       // primary template and partial specialization declarations
#include <dctl/board/dimensions.hpp>

namespace dctl {
namespace board {

// partial specialization definition
template<>
class Grid<0>
:
        public Dimensions
{
private:
        // range of even (e) and odd (o) rows

        constexpr auto row_e() const { return (width +  ul_parity(*this)) / 2; }
        constexpr auto row_o() const { return (width + !ul_parity(*this)) / 2; }

public:
        constexpr Grid(Dimensions const& dim) : Dimensions(dim) {}

        constexpr auto edge_columns() const { return 0; }

        // range of row pairs

        constexpr auto modulo() const { return width; }

        // left (l) and right (r) edges of even (e) and odd (o) rows

        constexpr auto edge_le() const { return 0; }
        constexpr auto edge_re() const { return edge_le() + (row_e() - 1); }
        constexpr auto edge_lo() const { return edge_re() + 1; }
        constexpr auto edge_ro() const { return edge_lo() + (row_o() - 1); }

        // grid size

        constexpr int size() const { return (width * height) / 2 + (width * height * ul_parity(*this)) % 2; }
};

template<int EdgeColumns>
class Grid
:
        public Dimensions
{
private:
        Grid<0> BaseGrid;

public:
        constexpr Grid(Dimensions const& dim) : Dimensions(dim), BaseGrid{dim} {}

        constexpr auto edge_columns() const { return EdgeColumns; }

        // range of row pairs

        constexpr auto left_down() const { return (width + EdgeColumns) / 2; }
        constexpr auto right_down() const { return left_down() + 1; }
        constexpr auto modulo() const { return left_down() + right_down(); }

        // left (l) and right (r) edges of even (e) and odd (o) rows

        constexpr auto edge_le() const { return BaseGrid.edge_le(); }
        constexpr auto edge_re() const { return BaseGrid.edge_re(); }
        constexpr auto edge_lo() const { return left_down() + ul_parity(*this); }
        constexpr auto edge_ro() const { return edge_lo() + (BaseGrid.edge_ro() - BaseGrid.edge_lo()); }

        // grid size

        constexpr auto size() const { return modulo() * ((height - 1) / 2) + ((height % 2) ? edge_re() : edge_ro()) + 1; }
};

}       // namespace board
}       // namespace dctl
