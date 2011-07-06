#pragma once
#include "Traits.h"

namespace rules {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc
struct International {};   

// aliases for the international rules on 8x8, 10x10 and 12x12 boards
typedef International Brazilian;
typedef International Polish;
typedef International Canadian;

// capture precedence
template<> struct is_majority_precedence<International> { enum { value = true  }; };

}       // namespace rules

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "../Tree/Generate/Capture/International.hpp"