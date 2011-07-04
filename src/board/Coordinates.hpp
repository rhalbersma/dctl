#include "Angles.h"

namespace board {
namespace coordinates {

template<int R, int C>
struct Pair
{
        enum {
                ROW = R,
                COL = C
        };
};

// identity rotation
template<typename T, typename In>
struct Rotate<T, In, angles::D000>
{
        typedef In Out;
};

// rotate 90 degrees right
template<typename T, typename In>
struct Rotate<T, In, angles::D270>
{
        typedef Pair
        <
                (T::WIDTH - 1) - In::COL, 
                In::ROW
        > Out;
};

// rotate 90 degrees left
template<typename T, typename In>
struct Rotate<T, In, angles::D090>
{
        typedef Pair
        <
                In::COL, 
                (T::HEIGHT - 1) - In::ROW
        > Out;
};

// rotate 180 degrees
template<typename T, typename In>
struct Rotate<T, In, angles::D180>
{
        typedef Pair
        <
                (T::HEIGHT - 1) - In::ROW, 
                (T::WIDTH - 1) - In::COL
        > Out;
};

template<typename T, int SQ>
class FromRange
{
private:
        enum {
                Q = SQ / T::MODULO,             // number of row pairs                     
                R0 = SQ % T::MODULO,            // squares from the left edge of the zeroth row
                R1 = R0 - T::EDGE_LO,           // squares from the left edge of the first row
                P = R1 >= 0,                    // determine whether R0 is in the zeroth or first row
                R = P? R1 : R0                  // squares from the left edge
        };

public:
        typedef Pair
        <
                2 * Q + P,                      // 2x the row pairs + the row parity
                2 * R + (P ^ !T::PARITY)        // 2x the range from the left edge + the row parity XOR the opposite board coloring
        > Out;
};

template<typename T, typename In>
class ToRange
{
private:
        enum {
                P = In::ROW % 2,                // row parity
                Q = In::ROW / 2,                // number of row pairs
                L = P? T::EDGE_LO : T::EDGE_LE, // the left edge
                S = In::COL / 2,                // number of column pairs
                R = (L + S) % T::MODULO         // squares from the left edge
        };

public:
        static const int VALUE = T::MODULO * Q + R;
};

}       // namespace coordinates
}       // namespace board
