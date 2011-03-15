#include "Angles.h"

namespace Geometry {
namespace Direction {

// the 8 diagonal and orthogonal direction indices for black and white to move
template<typename Board, bool Color> 
class Indices
{ 
private:                        
        enum { ANGLE = (Angles::D180 * !Color) + Board::A_PRIME };

public:
        enum {
                RIGHT       = Angles::Rotate<Angles::D000, ANGLE>::VALUE,
                RIGHT_UP    = Angles::Rotate<Angles::D045, ANGLE>::VALUE,
                UP          = Angles::Rotate<Angles::D090, ANGLE>::VALUE,
                LEFT_UP     = Angles::Rotate<Angles::D135, ANGLE>::VALUE,
                LEFT        = Angles::Rotate<Angles::D180, ANGLE>::VALUE,
                LEFT_DOWN   = Angles::Rotate<Angles::D225, ANGLE>::VALUE,
                DOWN        = Angles::Rotate<Angles::D270, ANGLE>::VALUE,
                RIGHT_DOWN  = Angles::Rotate<Angles::D315, ANGLE>::VALUE
        };
};

template<size_t I>
class Traits
{
private:
        static const size_t I_L090 = Rotate<I>::L090;

public:
        // diagonality, orthogonality
        static const bool IS_DIAGONAL = I % 2;                                  // 1, 3, 5, 7
        static const bool IS_ORTHGONAL = !IS_DIAGONAL;                         // 0, 2, 4, 6

        // up, down, right, left
        static const bool IS_UP = !(I / 4) && (I % 4);                          // 1, 2, 3
        static const bool IS_DOWN = (I / 4) && (I % 4);                         // 5, 6, 7
        static const bool IS_RIGHT = Traits<I_L090>::IS_UP;                     // 0, 1, 7
        static const bool IS_LEFT = Traits<I_L090>::IS_DOWN;                    // 3, 4, 5

        // positive, negative
        static const bool IS_POSITIVE = IS_UP || (IS_LEFT && !IS_DOWN);         // 1, 2, 3, 4
        static const bool IS_NEGATIVE = !IS_POSITIVE;                           // 5, 6, 7, 0
};

// rotated direction indices (in steps of 45 degrees anti-clockwise)
template<size_t I>
struct Rotate
{
        enum { 
                I000 = Angles::Rotate<I, Angles::D000>::VALUE, 
                L045 = Angles::Rotate<I, Angles::D045>::VALUE, 
                L090 = Angles::Rotate<I, Angles::D090>::VALUE, 
                L135 = Angles::Rotate<I, Angles::D135>::VALUE, 
                I180 = Angles::Rotate<I, Angles::D180>::VALUE, 
                R135 = Angles::Rotate<I, Angles::D225>::VALUE, 
                R090 = Angles::Rotate<I, Angles::D270>::VALUE, 
                R045 = Angles::Rotate<I, Angles::D315>::VALUE, 
                I360 = Angles::Rotate<I, Angles::D360>::VALUE
        };
};

// mirrored forward direction index (orthogonal to the original)
template<size_t I>
struct MirrorUp
{
        enum { VALUE = (I + Angles::D090) % Angles::D180 + (I / Angles::D180) * Angles::D180 };
};

// mirrored backward direction index (orthogonal to the original)
template<size_t I>
struct MirrorDown
{
        enum { VALUE = Rotate<MirrorUp<Rotate<I>::L090>::VALUE>::R090 };
};

}       // namespace Direction
}       // namespace Geometry
