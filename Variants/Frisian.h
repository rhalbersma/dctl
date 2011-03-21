#pragma once
#include "Traits.h"

namespace Variants {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct Frisian {};

template<> struct man_capture_directions<Frisian>               { enum { value = DIRS_ALL }; };
template<> struct king_capture_directions<Frisian>              { enum { value = DIRS_ALL }; };
template<> struct large_capture<Frisian>                        { enum { value = 3        }; };
template<> struct is_restricted_same_king_moves<Frisian>        { enum { value = true     }; };
template<> struct is_relative_king_precedence<Frisian>          { enum { value = true     }; };
template<> struct is_majority_precedence<Frisian>               { enum { value = true     }; };

}       // namespace Variants
