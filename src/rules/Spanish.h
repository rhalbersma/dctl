#pragma once
#include "Rules.h"

namespace dctl {
namespace rules {

// http://fpdamas.home.sapo.pt/regrasclass.htm
struct Spanish {};         

// move mechanics
template<> struct man_capture_directions<Spanish>       { enum { value = DIRS_UP }; };

// capture precedence
template<> struct is_majority_precedence<Spanish>       { enum { value = true    }; };

}       // namespace rules
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "../generate/capture/Spanish.hpp"
