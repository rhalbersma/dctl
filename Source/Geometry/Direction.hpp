#include "Angles.h"

namespace geometry {
namespace direction {

// the 8 diagonal and orthogonal direction indices for black and white to move
template<typename Board, bool Color> 
class Indices
{ 
private:                        
        enum { 
                ANGLE = (angles::D180 * !Color) + Board::A_PRIME 
        };

public:
        enum {
                RIGHT       = angles::Rotate<angles::D000, ANGLE>::VALUE,
                RIGHT_UP    = angles::Rotate<angles::D045, ANGLE>::VALUE,
                UP          = angles::Rotate<angles::D090, ANGLE>::VALUE,
                LEFT_UP     = angles::Rotate<angles::D135, ANGLE>::VALUE,
                LEFT        = angles::Rotate<angles::D180, ANGLE>::VALUE,
                LEFT_DOWN   = angles::Rotate<angles::D225, ANGLE>::VALUE,
                DOWN        = angles::Rotate<angles::D270, ANGLE>::VALUE,
                RIGHT_DOWN  = angles::Rotate<angles::D315, ANGLE>::VALUE
        };
};

template<int I>
class Traits
{
private:
        static const int I_L090 = Rotate<I>::L090;

public:
        // diagonality, orthogonality
        static const bool IS_DIAGONAL = I % 2;                          // 1, 3, 5, 7
        static const bool IS_ORTHGONAL = !IS_DIAGONAL;                  // 0, 2, 4, 6

        // up, down, right, left
        static const bool IS_UP = !(I / 4) && (I % 4);                  // 1, 2, 3
        static const bool IS_DOWN = (I / 4) && (I % 4);                 // 5, 6, 7
        static const bool IS_RIGHT = Traits<I_L090>::IS_UP;             // 0, 1, 7
        static const bool IS_LEFT = Traits<I_L090>::IS_DOWN;            // 3, 4, 5

        // positive, negative
        static const bool IS_POSITIVE = IS_UP || (IS_LEFT && !IS_DOWN); // 1, 2, 3, 4
        static const bool IS_NEGATIVE = !IS_POSITIVE;                   // 5, 6, 7, 0
};

// rotated direction indices (in steps of 45 degrees anti-clockwise)
template<int I>
struct Rotate
{
        enum { 
                I000 = angles::Rotate<I, angles::D000>::VALUE, 
                L045 = angles::Rotate<I, angles::D045>::VALUE, 
                L090 = angles::Rotate<I, angles::D090>::VALUE, 
                L135 = angles::Rotate<I, angles::D135>::VALUE, 
                I180 = angles::Rotate<I, angles::D180>::VALUE, 
                R135 = angles::Rotate<I, angles::D225>::VALUE, 
                R090 = angles::Rotate<I, angles::D270>::VALUE, 
                R045 = angles::Rotate<I, angles::D315>::VALUE, 
                I360 = angles::Rotate<I, angles::D360>::VALUE
        };
};

// mirrored forward direction index (orthogonal to the original)
template<int I>
struct MirrorUp
{
        enum { VALUE = (I + angles::D090) % angles::D180 + (I / angles::D180) * angles::D180 };
};

// mirrored backward direction index (orthogonal to the original)
template<int I>
struct MirrorDown
{
        enum { VALUE = Rotate<MirrorUp<Rotate<I>::L090>::VALUE>::R090 };
};

}       // namespace direction
}       // namespace geometry
