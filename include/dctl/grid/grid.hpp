#pragma once
#include <dctl/grid/grid_fwd.hpp>       // primary template and partial specialization declarations
#include <dctl/grid/dimensions.hpp>

namespace dctl {
namespace grid {

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


// primary template definition
template<class Dimensions, int EdgeColumns>
class GridClass
:
        public Dimensions
{
private:
        using BaseGrid = GridClass<Dimensions, 0>;

public:
        static constexpr auto edge_columns = EdgeColumns;

        // range of row pairs

        static constexpr auto left_down = (Dimensions::width + EdgeColumns) / 2;
        static constexpr auto right_down = left_down + 1;
        static constexpr auto modulo = left_down + right_down;

        // left (l) and right (r) edges of even (e) and odd (o) rows

        static constexpr auto edge_le = BaseGrid::edge_le;
        static constexpr auto edge_re = BaseGrid::edge_re;
        static constexpr auto edge_lo = left_down + Dimensions::ul_parity;
        static constexpr auto edge_ro = edge_lo + (BaseGrid::edge_ro - BaseGrid::edge_lo);

        // grid size

        static constexpr auto size = modulo * ((Dimensions::height - 1) / 2) + ((Dimensions::height % 2) ? edge_re : edge_ro) + 1;
};

// partial specialization definition
template<class Dimensions>
class GridClass<Dimensions, 0>
:
        public Dimensions
{
private:
        // range of even (e) and odd (o) rows

        static constexpr auto row_e = (Dimensions::width +  Dimensions::ul_parity) / 2;
        static constexpr auto row_o = (Dimensions::width + !Dimensions::ul_parity) / 2;

public:
        static constexpr auto edge_columns = 0;

        // range of row pairs

        static constexpr auto modulo = Dimensions::width;

        // left (l) and right (r) edges of even (e) and odd (o) rows

        static constexpr auto edge_le = 0;
        static constexpr auto edge_re = edge_le + (row_e - 1);
        static constexpr auto edge_lo = edge_re + 1;
        static constexpr auto edge_ro = edge_lo + (row_o - 1);

        // grid size

        static constexpr int size = (Dimensions::width * Dimensions::height) / 2 + (Dimensions::width * Dimensions::height * Dimensions::ul_parity) % 2;
};

template<class Dimensions>
constexpr int GridClass<Dimensions, 0>::size;

}       // namespace grid
}       // namespace dctl
