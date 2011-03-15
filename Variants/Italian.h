#pragma once
#include "Traits.h"

namespace Variants {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
struct Italian {};

template<> struct ManCaptureDirections<Italian>         { enum { VALUE = DIRS_UP }; };
template<> struct is_LongKingRange<Italian>             { enum { VALUE = RANGE_1 }; };
template<> struct is_MenCaptureKings<Italian>           { enum { VALUE = false   }; };
template<> struct is_RelativeKingPrecedence<Italian>    { enum { VALUE = true    }; };
template<> struct is_MajorityPrecedence<Italian>        { enum { VALUE = true    }; };

}       // namespace Variants
