#pragma once
#include "Traits.h"

namespace Variants {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc
struct International {};   

// aliases for the international rules on 8x8, 10x10 and 12x12 boards
typedef International Brazilian;
typedef International Polish;
typedef International Canadian;

template<> struct is_MajorityPrecedence<International>  { enum { VALUE = true  }; };

}       // namespace Variants
