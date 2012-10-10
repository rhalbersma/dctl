#include "Angle.h"

namespace dctl {
namespace board {

template<typename C>
class Coordinates2Square
{
private:
        typedef typename C::grid G;

        enum {
                P = C::row % 2,                 // row parity
                Q = C::row / 2,                 // number of row pairs
                L = P? G::EDGE_LO : G::EDGE_LE, // the left edge
                S = C::col / 2,                 // number of column pairs
                R = (L + S) % G::MODULO         // squares from the left edge
        };

public:
        typedef Square 
        <
                G,
                G::MODULO * Q + R
        > type;
};

template<typename SQ>
class Square2Coordinates
{
private:
        typedef typename SQ::grid G;

        enum {
                Q = SQ::square / G::MODULO,     // number of row pairs                     
                R0 = SQ::square % G::MODULO,    // squares from the left edge of the zeroth row
                R1 = R0 - G::EDGE_LO,           // squares from the left edge of the first row
                P = R1 >= 0,                    // determine whether R0 is in the zeroth or first row
                R = P? R1 : R0                  // squares from the left edge
        };

public:
        typedef Coordinates
        <
                G,
                2 * Q + P,                      // 2x the row pairs + the row parity
                2 * R + (P ^ !G::PARITY)        // 2x the range from the left edge + the row parity XOR the opposite board coloring
        > type;
};

// partial specialization for identity rotations
template<typename G, int R, int C>
struct Rotate<Coordinates<G, R, C>, Angle::D000>
{
        typedef Coordinates<G, R, C> type;
};

// partial specialization for 90 degrees right rotations
template<typename G, int R, int C>
struct Rotate<Coordinates<G, R, C>, Angle::D270>
{
        typedef Coordinates<G, (G::WIDTH - 1) - C, R> type;
};

// partial specialization for 90 degrees left rotations
template<typename G, int R, int C>
struct Rotate<Coordinates<G, R, C>, Angle::D090>
{
        typedef Coordinates<G, C, (G::HEIGHT - 1) - R> type;
};

// partial specialization for 180 degrees rotations
template<typename G, int R, int C>
struct Rotate<Coordinates<G, R, C>, Angle::D180>
{
        typedef Coordinates<G, (G::HEIGHT - 1) - R, (G::WIDTH - 1) - C> type;
};

}       // namespace board
}       // namespace dctl
