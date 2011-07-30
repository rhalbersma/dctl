#pragma once
#include "Enum.h"

namespace dctl {
namespace rules {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct Frisian {};

// move mechanics
template<typename> struct is_restricted_same_king_moves;
template<> struct is_restricted_same_king_moves<Frisian>        { enum { value = true     }; };

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<Frisian>               { enum { value = DIRS_ALL }; };

template<typename> struct king_capture_directions;
template<> struct king_capture_directions<Frisian>              { enum { value = DIRS_ALL }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<Frisian>               { enum { value = true     }; };

template<typename> struct is_relative_king_precedence;
template<> struct is_relative_king_precedence<Frisian>          { enum { value = true     }; };

// implementation of move generation
template<typename> struct large_capture;
template<> struct large_capture<Frisian>                        { enum { value = 3        }; };

}       // namespace rules
}       // namespace dctl
