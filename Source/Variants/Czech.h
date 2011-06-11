#pragma once
#include "Traits.h"

namespace variants {

// http://www.damweb.cz/pravidla/cdfull.html
struct Czech {};

// capture mechanics
template<> struct man_capture_directions<Czech>         { enum { value = DIRS_UP }; };

// capture precedence
template<> struct is_absolute_king_precedence<Czech>    { enum { value = true    }; };

}       // namespace variants
