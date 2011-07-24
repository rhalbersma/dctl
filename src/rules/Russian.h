#pragma once
#include "Rules.h"

namespace dctl {
namespace rules {

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct Russian {};

// move mechanics
template<> struct promotion_condition<Russian>  { enum { value = PROMOTE_EP }; };

}       // namespace rules
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "../generate/capture/Russian.hpp"
#include "../protocol/notation/Russian.h"
