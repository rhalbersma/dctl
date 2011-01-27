#pragma once
#include "Traits.h"

namespace Variant {

// http://fpdamas.home.sapo.pt/regrasclass.htm
struct Spanish {};         

template<> struct ManCaptureDirections<Spanish>         { enum { VALUE = DIRS_2 }; };
template<> struct is_MajorityPrecedence<Spanish>        { enum { VALUE = true   }; };

}       // namespace Variant