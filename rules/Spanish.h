#pragma once
#include "Enum.h"

namespace dctl {

namespace variant { struct Spanish; }         

namespace rules {

// move mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<variant::Spanish>      { enum { value = dirs_up }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::Spanish>      { enum { value = true    }; };

}       // namespace rules
}       // namespace dctl
