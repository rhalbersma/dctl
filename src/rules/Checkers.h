#pragma once
#include "Rules.h"

namespace dctl {
namespace rules {

// http://www.usacheckers.com/rulesofcheckers.php
struct Checkers {};

// aliases for the rules of checkers
typedef Checkers English;
typedef Checkers American;

// move mechanics
template<> struct is_long_king_range<Checkers>          { enum { value = RANGE_1 }; };

// capture mechanics
template<> struct man_capture_directions<Checkers>      { enum { value = DIRS_UP }; };

}       // namespace rules
}       // namespace dctl
