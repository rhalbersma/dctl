#pragma once
#include "Enum.h"

namespace dctl {
namespace rules {

// http://fpdamas.home.sapo.pt/regrasclass.htm
struct Spanish {};         

// move mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<Spanish>       { enum { value = DIRS_UP }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<Spanish>       { enum { value = true    }; };

}       // namespace rules
}       // namespace dctl
