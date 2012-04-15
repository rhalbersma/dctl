#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Pool; }

namespace rules {

// move mechanics
template<typename> struct king_scan_range;
template<> struct king_scan_range<variant::Pool>                { enum { value = scan_N    }; };

// capture mechanics
template<typename> struct pawn_jump_directions;
template<> struct pawn_jump_directions<variant::Pool>           { enum { value = dirs_diag }; };

}       // namespace rules
}       // namespace dctl
