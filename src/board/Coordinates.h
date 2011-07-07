#pragma once

namespace board {

template<typename G, int R, int C>
struct Coordinates
{
        typedef G grid;
        enum { row = R, col = C };
};

template<typename G, int S>
struct Square
{
        typedef G grid;
        enum { square = S };
};

template<typename S>
class Square2Coordinates
{
private:
        typedef typename S::grid G;

        enum {
                Q = S::square / G::MODULO,      // number of row pairs                     
                R0 = S::square % G::MODULO,     // squares from the left edge of the zeroth row
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

}       // namespace board

// include template definitions inside header since "export" keyword is not supported by most C++ compilers
#include "Coordinates.hpp"
