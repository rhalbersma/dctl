#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Thai; }

namespace rules {

// move mechanics
template<typename> struct is_long_king_range;
template<> struct is_long_king_range<variant::Thai>             { enum { value = range_N   }; };

// capture mechanics
template<typename> struct is_capture_direction_reversal;
template<> struct is_capture_direction_reversal<variant::Thai>  { enum { value = true      }; };

template<typename> struct king_capture_halt;
template<> struct king_capture_halt<variant::Thai>              { enum { value = halt_1    }; };

template<typename> struct capture_removal;
template<> struct capture_removal<variant::Thai>                { enum { value = remove_ep }; };

// capture ambiguity
template<typename> struct large_capture;
template<> struct large_capture<variant::Thai>                  { enum { value = 3         }; };

}       // namespace rules
}       // namespace dctl
