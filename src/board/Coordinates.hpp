#include "Angle.h"

namespace dctl {
namespace board {

template<typename C>
class Coordinates2Square
{
private:
        typedef typename C::grid G;

        BOOST_STATIC_CONSTANT(auto, P = C::row % 2);                    // row parity
        BOOST_STATIC_CONSTANT(auto, Q = C::row / 2);                    // number of row pairs
        BOOST_STATIC_CONSTANT(auto, L = P? G::edge_lo : G::edge_le);    // the left edge
        BOOST_STATIC_CONSTANT(auto, S = C::col / 2);                    // number of column pairs
        BOOST_STATIC_CONSTANT(auto, R = (L + S) % G::modulo);           // squares from the left edge

public:
        typedef Square 
        <
                G,
                G::modulo * Q + R
        > type;
};

template<typename SQ>
class Square2Coordinates
{
private:
        typedef typename SQ::grid G;

        BOOST_STATIC_CONSTANT(auto, Q = SQ::square / G::modulo);        // number of row pairs                     
        BOOST_STATIC_CONSTANT(auto, R0 = SQ::square % G::modulo);       // left edge of the zeroth row
        BOOST_STATIC_CONSTANT(auto, R1 = R0 - G::edge_lo);              // left edge of the first row
        BOOST_STATIC_CONSTANT(auto, P = R1 >= 0);                       // R0 is in the zeroth or first row
        BOOST_STATIC_CONSTANT(auto, R = P? R1 : R0);                    // squares from the left edge

public:
        typedef Coordinates
        <
                G,
                2 * Q + P,                      // 2x the row pairs + the row parity
                2 * R + (P ^ !G::parity)        // 2x the range from the left edge + the row parity XOR the opposite board coloring
        > type;
};

}       // namespace board

// partial specialization for identity rotations
template<typename Grid, int Row, int Column>
struct rotate<board::Coordinates<Grid, Row, Column>, Degrees::D000>
{
        typedef board::Coordinates<Grid, Row, Column> type;
};

// partial specialization for 90 degrees right (clockwise) rotations
template<typename Grid, int Row, int Column>
struct rotate<board::Coordinates<Grid, Row, Column>, Degrees::D270>
{
        typedef board::Coordinates<Grid, (Grid::width - 1) - Column, Row> type;
};

// partial specialization for 90 degrees left (counter-clockwise) rotations
template<typename Grid, int Row, int Column>
struct rotate<board::Coordinates<Grid, Row, Column>, Degrees::D090>
{
        typedef board::Coordinates<Grid, Column, (Grid::height - 1) - Row> type;
};

// partial specialization for 180 degrees rotations
template<typename Grid, int Row, int Column>
struct rotate<board::Coordinates<Grid, Row, Column>, Degrees::D180>
{
        typedef board::Coordinates<Grid, (Grid::height - 1) - Row, (Grid::width - 1) - Column> type;
};

}       // namespace dctl
