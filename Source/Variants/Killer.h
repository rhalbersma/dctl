#pragma once
#include "Traits.h"

namespace Variants {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
struct Killer {};

// capture mechanics
template<> struct king_capture_halt<Killer>             { enum { value = HALT_K }; };

// capture precedence
template<> struct is_majority_precedence<Killer>        { enum { value = true   }; };

}       // namespace Variants
