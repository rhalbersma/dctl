#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Killer; }
        
namespace rules {

// move mechanics
template<typename> struct king_scan_range;
template<> struct king_scan_range<variant::Killer>              { enum { value = scan_N    }; };

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<variant::Killer>       { enum { value = dirs_diag }; };

template<typename> struct king_capture_halt;
template<> struct king_capture_halt<variant::Killer>            { enum { value = halt_1K   }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::Killer>       { enum { value = true      }; };

}       // namespace rules
}       // namespace dctl
