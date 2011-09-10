#pragma once
#include "Enum.h"

namespace dctl {

namespace variant { struct Italian; }

namespace rules {

// move mechanics
template<typename> struct is_long_king_range;
template<> struct is_long_king_range<variant::Italian>          { enum { value = RANGE_1 }; };

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<variant::Italian>      { enum { value = DIRS_up }; };

template<typename> struct is_men_capture_kings;
template<> struct is_men_capture_kings<variant::Italian>        { enum { value = false   }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::Italian>      { enum { value = true    }; };

template<typename> struct is_relative_king_precedence;
template<> struct is_relative_king_precedence<variant::Italian> { enum { value = true    }; };

}       // namespace rules
}       // namespace dctl
