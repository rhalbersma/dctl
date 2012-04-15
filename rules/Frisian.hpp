#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Frisian; }

namespace rules {

// move mechanics
template<typename> struct king_scan_range;
template<> struct king_scan_range<variant::Frisian>                     { enum { value = scan_N   }; };

template<typename> struct is_restricted_same_king_moves;
template<> struct is_restricted_same_king_moves<variant::Frisian>       { enum { value = true     }; };

template<typename> struct max_same_king_moves;
template<> struct max_same_king_moves<variant::Frisian>                 { enum { value = 6        }; };

// capture mechanics
template<typename> struct pawn_jump_directions;
template<> struct pawn_jump_directions<variant::Frisian>                { enum { value = dirs_all }; };

template<typename> struct king_jump_directions;
template<> struct king_jump_directions<variant::Frisian>                { enum { value = dirs_all }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::Frisian>              { enum { value = true     }; };

template<typename> struct is_qualified_majority;
template<> struct is_qualified_majority<variant::Frisian>               { enum { value = true     }; };

template<typename> struct is_relative_king_precedence;
template<> struct is_relative_king_precedence<variant::Frisian>         { enum { value = true     }; };

// capture ambiguity
template<typename> struct large_capture;
template<> struct large_capture<variant::Frisian>                       { enum { value = 3        }; };

}       // namespace rules
}       // namespace dctl
