#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Spanish; }         

namespace rules {

// move mechanics
template<typename> struct is_long_king_range;
template<> struct is_long_king_range<variant::Spanish>          { enum { value = range_N }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::Spanish>      { enum { value = true    }; };

}       // namespace rules
}       // namespace dctl
