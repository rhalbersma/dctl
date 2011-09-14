#pragma once
#include "Enum.h"

namespace dctl {

namespace variant { struct Killer; }
        
namespace rules {

// capture mechanics
template<typename> struct king_capture_halt;
template<> struct king_capture_halt<variant::Killer>            { enum { value = halt_1K }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::Killer>       { enum { value = true   }; };

}       // namespace rules
}       // namespace dctl
