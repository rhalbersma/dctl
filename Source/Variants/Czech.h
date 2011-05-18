#pragma once
#include "Traits.h"

namespace Variants {

// http://www.damweb.cz/pravidla/cdfull.html
struct Czech {};

template<> struct man_capture_directions<Czech>         { enum { value = DIRS_UP }; };
template<> struct is_absolute_king_precedence<Czech>    { enum { value = true    }; };

}       // namespace Variants
