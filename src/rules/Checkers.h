#pragma once
#include "Enum.h"

namespace dctl {
namespace rules {

// http://www.usacheckers.com/rulesofcheckers.php
struct Checkers {};

// aliases for the rules of checkers
typedef Checkers English;
typedef Checkers American;

// move mechanics
template<typename> struct is_long_king_range;
template<> struct is_long_king_range<Checkers>          { enum { value = RANGE_1 }; };

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<Checkers>      { enum { value = DIRS_UP }; };

}       // namespace rules
}       // namespace dctl
