#pragma once
#include "Enum.h"

namespace dctl {

namespace variant { struct Checkers; }
        
namespace rules {

// move mechanics
template<typename> struct is_long_king_range;
template<> struct is_long_king_range<variant::Checkers>        { enum { value = range_1 }; };

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<variant::Checkers>    { enum { value = dirs_up }; };

}       // namespace rules
}       // namespace dctl
