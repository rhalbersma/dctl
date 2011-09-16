#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Pool; }
        
namespace rules {

// move mechanics
template<typename> struct is_long_king_range;
template<> struct is_long_king_range<variant::Pool>        { enum { value = range_N   }; };

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<variant::Pool>    { enum { value = dirs_diag }; };

}       // namespace rules
}       // namespace dctl
