#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Russian; }

namespace rules {

// move mechanics
template<typename> struct is_long_king_range;
template<> struct is_long_king_range<variant::Russian>          { enum { value = range_N    }; };

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<variant::Russian>      { enum { value = dirs_diag  }; };

template<typename> struct promotion_condition;
template<> struct promotion_condition<variant::Russian>         { enum { value = promote_ep }; };

}       // namespace rules
}       // namespace dctl
