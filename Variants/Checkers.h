#pragma once
#include "Traits.h"

namespace Variants {

// http://www.usacheckers.com/rulesofcheckers.php
struct Checkers {};

// aliases for the rules of checkers
typedef Checkers English;
typedef Checkers American;

template<> struct man_capture_directions<Checkers>      { enum { value = DIRS_UP }; };
template<> struct is_long_king_range<Checkers>          { enum { value = RANGE_1 }; };

}       // namespace Variants
