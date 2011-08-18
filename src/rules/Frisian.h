#pragma once
#include "Enum.h"

namespace dctl {

namespace variant { struct Frisian; }

namespace rules {

// move mechanics
template<typename> struct is_restricted_same_king_moves;
template<> struct is_restricted_same_king_moves<variant::Frisian>       { enum { value = true     }; };

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<variant::Frisian>              { enum { value = DIRS_ALL }; };

template<typename> struct king_capture_directions;
template<> struct king_capture_directions<variant::Frisian>             { enum { value = DIRS_ALL }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::Frisian>              { enum { value = true     }; };

template<typename> struct is_relative_king_precedence;
template<> struct is_relative_king_precedence<variant::Frisian>         { enum { value = true     }; };

// capture ambiguity
template<typename> struct large_capture;
template<> struct large_capture<variant::Frisian>                       { enum { value = 3        }; };

}       // namespace rules
}       // namespace dctl
