#pragma once
#include "Traits.h"

namespace Variants {

// http://fpdamas.home.sapo.pt/regrasclass.htm
struct Spanish {};         

template<> struct ManCaptureDirections<Spanish>         { enum { VALUE = DIRS_UP }; };
template<> struct is_MajorityPrecedence<Spanish>        { enum { VALUE = true    }; };

}       // namespace Variants
