#pragma once
#include "Traits.h"

namespace rules {

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct Russian {};

// move mechanics
template<> struct promotion_condition<Russian>  { enum { value = PROMOTE_EP }; };

}       // namespace rules

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "../Tree/Generate/Capture/Russian.hpp"
