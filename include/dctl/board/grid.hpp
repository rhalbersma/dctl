#pragma once
#include <dctl/board/grid_fwd.hpp>      // primary template and partial specialization declarations
#include <dctl/board/edge.hpp>          // ColumnLessEdge

namespace dctl {
namespace board {

// primary template definition
template<class Dimensions, class Edge>
struct Grid
:
        public Dimensions, public Edge
{
public:
        using BaseGrid = Grid<Dimensions, ColumnLessEdge>;

        // diagonal directions

        static constexpr auto left_down = (Dimensions::width + Edge::num_columns) / 2;
        static constexpr auto right_down = left_down + 1;

        // orthogonal directions

        static constexpr auto right = right_down - left_down;
        static constexpr auto down = right_down + left_down;

        // equivalent directions

        static constexpr auto left_up = right_down;
        static constexpr auto right_up = left_down;
        static constexpr auto left = right;
        static constexpr auto up = down;

        // range of row pairs

        static constexpr auto modulo = down;

        // left (l) and right (r) edges of even (e) and odd (o) rows

        static constexpr auto edge_le = BaseGrid::edge_le;
        static constexpr auto edge_re = BaseGrid::edge_re;
        static constexpr auto edge_lo = left_down + Dimensions::parity;
        static constexpr auto edge_ro = edge_lo + (BaseGrid::edge_ro - BaseGrid::edge_lo);

        // grid size

        static constexpr auto size = modulo * (Dimensions::height / 2) + (Dimensions::height % 2) * (edge_re + 1);
};

// partial specialization definition
template<class Dimensions>
struct Grid<Dimensions, ColumnLessEdge>
:
        public Dimensions
{
private:
        // range of even (e) and odd (o) rows

        static constexpr auto row_e = (Dimensions::width +  Dimensions::parity) / 2;
        static constexpr auto row_o = (Dimensions::width + !Dimensions::parity) / 2;

public:
        // range of row pairs

        static constexpr auto modulo = Dimensions::width;

        // left (l) and right (r) edges of even (e) and odd (o) rows

        static constexpr auto edge_le = 0;
        static constexpr auto edge_re = edge_le + (row_e - 1);
        static constexpr auto edge_lo = edge_re + 1;
        static constexpr auto edge_ro = edge_lo + (row_o - 1);

        // grid size

        static constexpr auto size = modulo * (Dimensions::height / 2) + (Dimensions::height % 2) * (edge_re + 1);
        static_assert(size == (Dimensions::width * Dimensions::height) / 2 + (Dimensions::width * Dimensions::height * Dimensions::parity) % 2, "");
};

}       // namespace board
}       // namespace dctl
