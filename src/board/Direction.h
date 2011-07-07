#pragma once
#include "../Utilities/TemplateTricks.h"

namespace board {

// the 8 diagonal and orthogonal direction indices for black and white to move
template<bool Color, typename Board> 
class Direction
{ 
private:                        
        enum { A = (Angle::D180 * !Color) + Board::A_PRIME };

public:
        enum {
                RIGHT      = Rotate<Int2Type<Angle::D000>, A>::value,
                RIGHT_UP   = Rotate<Int2Type<Angle::D045>, A>::value,
                UP         = Rotate<Int2Type<Angle::D090>, A>::value,
                LEFT_UP    = Rotate<Int2Type<Angle::D135>, A>::value,
                LEFT       = Rotate<Int2Type<Angle::D180>, A>::value,
                LEFT_DOWN  = Rotate<Int2Type<Angle::D225>, A>::value,
                DOWN       = Rotate<Int2Type<Angle::D270>, A>::value,
                RIGHT_DOWN = Rotate<Int2Type<Angle::D315>, A>::value
        };
};

}       // namespace board
