#pragma once
#include "Enum.h"

namespace dctl {

namespace variant { struct Czech; }

namespace rules {

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<variant::Czech>       { enum { value = DIRS_up }; };

// capture precedence
template<typename> struct is_absolute_king_precedence;
template<> struct is_absolute_king_precedence<variant::Czech>  { enum { value = true    }; };

}       // namespace rules
}       // namespace dctl
