#pragma once
#include "Traits.h"

namespace rules {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct Frisian {};

// move mechanics
template<> struct is_restricted_same_king_moves<Frisian>        { enum { value = true     }; };

// capture mechanics
template<> struct man_capture_directions<Frisian>               { enum { value = DIRS_ALL }; };
template<> struct king_capture_directions<Frisian>              { enum { value = DIRS_ALL }; };

// capture precedence
template<> struct is_majority_precedence<Frisian>               { enum { value = true     }; };
template<> struct is_relative_king_precedence<Frisian>          { enum { value = true     }; };

// implementation of move generation
template<> struct large_capture<Frisian>                        { enum { value = 3        }; };

}       // namespace rules

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Frisian.hpp"
