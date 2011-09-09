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

// partial specialization for identity rotations
template<typename G, int R, int C>
struct rotate<Coordinates<G, R, C>, Angle::D000>
{
        typedef Coordinates<G, R, C> type;
};

// partial specialization for 90 degrees right rotations
template<typename G, int R, int C>
struct rotate<Coordinates<G, R, C>, Angle::D270>
{
        typedef Coordinates<G, (G::width - 1) - C, R> type;
};

// partial specialization for 90 degrees left rotations
template<typename G, int R, int C>
struct rotate<Coordinates<G, R, C>, Angle::D090>
{
        typedef Coordinates<G, C, (G::height - 1) - R> type;
};

// partial specialization for 180 degrees rotations
template<typename G, int R, int C>
struct rotate<Coordinates<G, R, C>, Angle::D180>
{
        typedef Coordinates<G, (G::height - 1) - R, (G::width - 1) - C> type;
};

}       // namespace board
}       // namespace dctl
