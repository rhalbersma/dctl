#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Pool; }
        
namespace rules {

// move mechanics
template<typename> struct king_scan_range;
template<> struct king_scan_range<variant::Pool>        { enum { value = scan_N    }; };

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<variant::Pool> { enum { value = dirs_diag }; };

}       // namespace rules
}       // namespace dctl