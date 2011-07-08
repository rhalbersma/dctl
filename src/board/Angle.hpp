#include "../Utilities/TemplateTricks.h"

namespace board {

// partial specialization for angles
template<int I, int A>
struct Rotate<Int2Type<I>, A>
{
        // angles are subject to arithmetic modulo 360 degrees, 
        // with the unit element equal to 45 degrees anti-clockwise
        enum { value = (I + A) % Angle::D360 };
};

// an inverse angle has the opposite sign modulo 360 degrees
template<int I>
struct Inverse
{
        enum { value = (Angle::D360 - I) % Angle::D360 };
};

// mirrored forward direction index (orthogonal to the original)
template<int I>
struct MirrorUp
{
        enum { value = (I + Angle::D090) % Angle::D180 + (I / Angle::D180) * Angle::D180 };
};

// mirrored backward direction index (orthogonal to the original)
template<int I>
struct MirrorDown
{
        // M' == R M L
        enum { value = Rotate<MirrorUp<Rotate<Int2Type<I>, Angle::L090>::value>::VALUE, Angle::R090>::value };
};

template<int I>
class Traits
{
private:
        static const int I_L090 = Rotate<Int2Type<I>, Angle::L090>::value;

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

}       // namespace board
