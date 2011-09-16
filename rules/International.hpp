#pragma once

namespace dctl {

namespace variant { struct International; }   

namespace rules {

// move mechanics
template<typename> struct is_long_king_range;
template<> struct is_long_king_range<variant::International>            { enum { value = range_N   }; };

// capture mechanics
template<typename> struct man_capture_directions;
template<> struct man_capture_directions<variant::International>        { enum { value = dirs_diag }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::International>        { enum { value = true      }; };

}       // namespace rules
}       // namespace dctl
