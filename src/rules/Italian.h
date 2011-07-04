#pragma once
#include "Traits.h"

namespace rules {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
struct Italian {};

// move mechanics
template<> struct is_long_king_range<Italian>           { enum { value = RANGE_1 }; };

// capture mechanics
template<> struct man_capture_directions<Italian>       { enum { value = DIRS_UP }; };
template<> struct is_men_capture_kings<Italian>         { enum { value = false   }; };

// capture precedence
template<> struct is_majority_precedence<Italian>       { enum { value = true    }; };
template<> struct is_relative_king_precedence<Italian>  { enum { value = true    }; };

}       // namespace rules
