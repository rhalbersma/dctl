#pragma once

namespace dctl {
namespace board {
namespace detail {

template<class T>
constexpr auto centralize(T const& t, T const& c)
{
        return 2 * t - (c - 1);
}

template<class T>
constexpr auto decentralize(T const& t, T const& c)
{
        return (t + (c - 1)) / 2;
}

}       // namespace detail

template<class Grid, int N>
struct Square
{
        using grid = Grid;
        static constexpr auto value = N;
};

template<class Grid>
struct xSquare
{
        int value;
};

template<int R, int C>
struct Coordinates
{
        static constexpr auto row = R;
        static constexpr auto col = C;

        using type = Coordinates<row, col>;
};

struct xCoordinates
{
        int row;
        int col;
};

template<class SQ>
struct Square2Coordinates
{
private:
        using grid = typename SQ::grid;

        // number of row pairs
        static constexpr auto Q = SQ::value / grid::modulo;

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

        static constexpr auto CENTRAL_ROW = detail::centralize(ROW, grid::height);
        static constexpr auto CENTRAL_COL = detail::centralize(COL, grid::width);

public:
        using type = Coordinates<CENTRAL_ROW, CENTRAL_COL>;
};

template<class Grid>
constexpr auto sqtocoord(xSquare<Grid> const& square) noexcept
{
        // number of row pairs
        constexpr auto Q = square.value / Grid::modulo;

        // left edge of the zeroth row
        constexpr auto R0 = square.value % Grid::modulo;

        // left edge of the first row
        constexpr auto R1 = R0 - Grid::edge_lo;

        // R0 is in the zeroth or first row
        constexpr auto P = R1 >= 0;

        // squares from the left edge
        constexpr auto R = P? R1 : R0;

        // 2x the row pairs + the row parity
        constexpr auto ROW = 2 * Q + P;

        // 2x the range from the left edge + the row parity XOR the opposite board coloring
        constexpr auto COL = 2 * R + (P ^ !Grid::parity);

        return xCoordinates
        {
                detail::centralize(ROW, Grid::height),
                detail::centralize(COL, Grid::width)
        };
}

template<class Grid, class Coord>
struct Coordinates2Square
{
private:
        static constexpr auto DECENTRAL_ROW = detail::decentralize(Coord::row, Grid::height);
        static constexpr auto DECENTRAL_COL = detail::decentralize(Coord::col, Grid::width);

        // row parity
        static constexpr auto P = DECENTRAL_ROW % 2;

        // number of row pairs
        static constexpr auto Q = DECENTRAL_ROW / 2;

        // the left edge
        static constexpr auto L = P? Grid::edge_lo : Grid::edge_le;

        // number of column pairs
        static constexpr auto S = DECENTRAL_COL / 2;

        // squares from the left edge
        static constexpr auto R = (L + S) % Grid::modulo;

        static constexpr auto NUM = Grid::modulo * Q + R;
public:
        using type = Square<Grid, NUM>;
};

template<class Grid>
constexpr auto coordtosq(xCoordinates const& coord)
{
        constexpr auto DECENTRAL_ROW = detail::decentralize(coord.row, Grid::height);
        constexpr auto DECENTRAL_COL = detail::decentralize(coord.col, Grid::width);

        // row parity
        constexpr auto P = DECENTRAL_ROW % 2;

        // number of row pairs
        constexpr auto Q = DECENTRAL_ROW / 2;

        // the left edge
        constexpr auto L = P? Grid::edge_lo : Grid::edge_le;

        // number of column pairs
        constexpr auto S = DECENTRAL_COL / 2;

        // squares from the left edge
        constexpr auto R = (L + S) % Grid::modulo;

        constexpr auto NUM = Grid::modulo * Q + R;

        return xSquare<Grid>{ NUM };
}

}       // namespace board
}       // namespace dctl
