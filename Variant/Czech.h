#pragma once
#include "Traits.h"

namespace Variant {

// http://www.damweb.cz/pravidla/cdfull.html
struct Czech {};

template<> struct ManCaptureDirections<Czech>           { enum { VALUE = DIRS_2 }; };
template<> struct is_AbsoluteKingPrecedence<Czech>      { enum { VALUE = true   }; };

}       // namespace Variant
