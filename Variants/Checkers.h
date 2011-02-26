#pragma once
#include "Traits.h"

namespace Variant {

// http://www.usacheckers.com/rulesofcheckers.php
struct Checkers {};

// aliases for the rules of checkers
typedef Checkers English;
typedef Checkers American;

template<> struct ManCaptureDirections<Checkers>        { enum { VALUE = DIRS_2  }; };
template<> struct is_LongKingRange<Checkers>            { enum { VALUE = RANGE_1 }; };

}       // namespace Variant
