#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Spanish; }         

namespace rules {

// move mechanics
template<typename> struct king_scan_range;
template<> struct king_scan_range<variant::Spanish>             { enum { value = scan_N  }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::Spanish>      { enum { value = true    }; };

template<typename> struct is_qualified_majority;
template<> struct is_qualified_majority<variant::Spanish>       { enum { value = true    }; };

}       // namespace rules
}       // namespace dctl
