#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Czech; }

namespace rules {

// move mechanics
template<typename> struct king_scan_range;
template<> struct king_scan_range<variant::Czech>               { enum { value = scan_N  }; };

// capture precedence
template<typename> struct is_absolute_king_precedence;
template<> struct is_absolute_king_precedence<variant::Czech>   { enum { value = true    }; };

}       // namespace rules
}       // namespace dctl
