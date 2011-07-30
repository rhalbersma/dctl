#pragma once
#include "Enum.h"

namespace dctl {
namespace rules {

// http://www.mindsports.nl/index.php/arena/draughts/382-killer-draughts-rules
struct Killer {};

// capture mechanics
template<typename> struct king_capture_halt;
template<> struct king_capture_halt<Killer>             { enum { value = HALT_K }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<Killer>        { enum { value = true   }; };

}       // namespace rules
}       // namespace dctl
