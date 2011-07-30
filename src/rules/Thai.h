#pragma once
#include "Enum.h"

namespace dctl {
namespace rules {

struct Thai {};

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<Thai>          { enum { value = DIRS_UP  }; };

template<typename> struct is_capture_direction_reversal;
template<> struct is_capture_direction_reversal<Thai>   { enum { value = true     }; };

template<typename> struct king_capture_halt;
template<> struct king_capture_halt<Thai>               { enum { value = HALT_1   }; };

template<typename> struct capture_removal;
template<> struct capture_removal<Thai>                 { enum { value = REMOVE_1 }; };

// implementation of move generation
template<typename> struct large_capture;
template<> struct large_capture<Thai>                   { enum { value = 3        }; };

}       // namespace rules
}       // namespace dctl
