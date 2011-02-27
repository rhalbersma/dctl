#pragma once
#include "Traits.h"

namespace Variants {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
struct Killer {};

template<> struct KingCaptureHalt<Killer>               { enum { VALUE = HALT_K }; };
template<> struct is_MajorityPrecedence<Killer>         { enum { VALUE = true   }; };

}       // namespace Variants
