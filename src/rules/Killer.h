#pragma once
#include "Traits.h"

namespace rules {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
struct Killer {};

// capture mechanics
template<> struct king_capture_halt<Killer>             { enum { value = HALT_K }; };

// capture precedence
template<> struct is_majority_precedence<Killer>        { enum { value = true   }; };

}       // namespace rules

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "../Tree/Generate/Capture/Killer.hpp"