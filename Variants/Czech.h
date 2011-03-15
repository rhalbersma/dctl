#pragma once
#include "Traits.h"

namespace Variants {

// http://www.damweb.cz/pravidla/cdfull.html
struct Czech {};

template<> struct ManCaptureDirections<Czech>           { enum { VALUE = DIRS_UP }; };
template<> struct is_AbsoluteKingPrecedence<Czech>      { enum { VALUE = true    }; };

}       // namespace Variants
