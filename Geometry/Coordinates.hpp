#include "Angles.h"

namespace Geometry {
namespace Coordinates {

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
struct Rotate<T, In, Angles::D000>
{
        typedef In Out;
};

// rotate 90 degrees right
template<typename T, typename In>
struct Rotate<T, In, Angles::D270>
{
        typedef Pair
        <
                (static_cast<int>(T::WIDTH) - 1) - In::COL, 
                In::ROW
        > Out;
};

// rotate 90 degrees left
template<typename T, typename In>
struct Rotate<T, In, Angles::D090>
{
        typedef Pair
        <
                In::COL, 
                (static_cast<int>(T::HEIGHT) - 1) - In::ROW
        > Out;
};

// rotate 180 degrees
template<typename T, typename In>
struct Rotate<T, In, Angles::D180>
{
        typedef Pair
        <
                (static_cast<int>(T::HEIGHT) - 1) - In::ROW, 
                (static_cast<int>(T::WIDTH) - 1) - In::COL
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
                P = R1 >= 0,                    // determine whether square R0 is in the zeroth or first row
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
                L0 = static_cast<int>(T::EDGE_LE),      // left edge of the zeroth row
                L1 = static_cast<int>(T::EDGE_LO),      // left edge of the first row
                M = static_cast<int>(T::MODULO),        // range for row pairs
                P = In::ROW % 2,                        // row parity
                Q = In::ROW / 2,                        // number of row pairs
                L = P? L1 : L0,                         // the left edge
                S = In::COL / 2,                        // number of column pairs
                R = (L + S) % M                         // squares from the left edge
        };

public:
        static const int VALUE = M * Q + R;
};

}       // namespace Coordinates
}       // namespace Geometry
