#pragma once
#include "Enum.h"

namespace dctl {

namespace variant { struct Russian; }

namespace rules {

// move mechanics
template<typename> struct promotion_condition;
template<> struct promotion_condition<variant::Russian> { enum { value = promote_ep }; };

}       // namespace rules
}       // namespace dctl
