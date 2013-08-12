#pragma once

namespace dctl {
namespace board {

template<class Grid, int N>
struct Square
{
        using grid = Grid;
        static constexpr auto value = N;
};

template<class Grid, int R, int C>
struct Coordinates
{
        using grid = Grid;
        static constexpr auto row = R;
        static constexpr auto col = C;

        using type = Coordinates<grid, R, C>;
};

template<class SQ>
struct Square2Coordinates
{
private:
        using grid = typename SQ::grid;

        // number of row pairs
        static constexpr auto Q =  SQ::value / grid::modulo;

        // left edge of the zeroth row
        static constexpr auto R0 = SQ::value % grid::modulo;

        // left edge of the first row
        static constexpr auto R1 = R0 - grid::edge_lo;

        // R0 is in the zeroth or first row
        static constexpr auto P = R1 >= 0;

        // squares from the left edge
        static constexpr auto R = P? R1 : R0;

        // 2x the row pairs + the row parity
        static constexpr auto ROW = 2 * Q + P;

        // 2x the range from the left edge + the row parity XOR the opposite board coloring
        static constexpr auto COL = 2 * R + (P ^ !grid::parity);

public:
        using type = Coordinates<grid, ROW, COL>;
};

template<class Coord>
struct Coordinates2Square
{
private:
        using grid = typename Coord::grid;

        // row parity
        static constexpr auto P = Coord::row % 2;

        // number of row pairs
        static constexpr auto Q = Coord::row / 2;

        // the left edge
        static constexpr auto L = P? grid::edge_lo : grid::edge_le;

        // number of column pairs
        static constexpr auto S = Coord::col / 2;

        // squares from the left edge
        static constexpr auto R = (L + S) % grid::modulo;

        static constexpr auto NUM = grid::modulo *  Q + R;
public:
        using type = Square<grid, NUM>;
};

}       // namespace board
}       // namespace dctl
