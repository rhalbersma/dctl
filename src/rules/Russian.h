#pragma once
#include "Enum.h"

namespace dctl {

namespace variant { struct Russian; }

namespace rules {

// move mechanics
template<typename> struct promotion_condition;
template<> struct promotion_condition<variant::Russian> { enum { value = PROMOTE_EP }; };

}       // namespace rules
}       // namespace dctl
