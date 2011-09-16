#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Czech; }

namespace rules {

// move mechanics
template<typename> struct is_long_king_range;
template<> struct is_long_king_range<variant::Czech>            { enum { value = range_N }; };

// capture precedence
template<typename> struct is_absolute_king_precedence;
template<> struct is_absolute_king_precedence<variant::Czech>   { enum { value = true    }; };

}       // namespace rules
}       // namespace dctl
