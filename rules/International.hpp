#pragma once

namespace dctl {

namespace variant { struct International; }

namespace rules {

// move mechanics
template<typename> struct king_scan_range;
template<> struct king_scan_range<variant::International>               { enum { value = scan_N    }; };

// capture mechanics
template<typename> struct pawn_jump_directions;
template<> struct pawn_jump_directions<variant::International>          { enum { value = dirs_diag }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::International>        { enum { value = true      }; };

}       // namespace rules
}       // namespace dctl
