#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Russian; }

namespace rules {

// move mechanics
template<typename> struct king_scan_range;
template<> struct king_scan_range<variant::Russian>             { enum { value = scan_N     }; };

// capture mechanics
template<typename> struct pawn_jump_directions;
template<> struct pawn_jump_directions<variant::Russian>        { enum { value = dirs_diag  }; };

template<typename> struct promotion_condition;
template<> struct promotion_condition<variant::Russian>         { enum { value = promote_ep }; };

}       // namespace rules
}       // namespace dctl
