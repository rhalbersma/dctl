#pragma once

namespace dctl {

struct Material
{
        enum { 
                none = 0, 
                pawn = 1, 
                king = pawn << 1, 
                both = pawn ^ king
        }; 
};

}       // namespace dctl
