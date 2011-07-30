#pragma once
#include "Enum.h"

namespace dctl {
namespace rules {

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct Russian {};

// move mechanics
template<typename> struct promotion_condition;
template<> struct promotion_condition<Russian>  { enum { value = PROMOTE_EP }; };

}       // namespace rules
}       // namespace dctl
