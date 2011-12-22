#pragma once
#include "Enum.hpp"

namespace dctl {
namespace rules {

//+----------------------------------------------------------------------------+
//|      Move mechanics                                                        |
//+----------------------------------------------------------------------------+

// king range
template<typename> struct king_scan_range                       { enum { value = scan_1 }; };

// restricted consecutive moves with the same king
template<typename> struct is_restricted_same_king_moves         { enum { value = false }; };

// maximum consecutive moves with the same king                 // NO default: MUST be specialized if
template<typename> struct max_same_king_moves;                  // is_restricted_same_king_moves == true 

//+----------------------------------------------------------------------------+
//|      Capture mechanics                                                     |
//+----------------------------------------------------------------------------+

// men can capture kings
template<typename> struct is_men_capture_kings                  { enum { value = true  }; };

// king capture directions
template<typename> struct king_capture_directions               { enum { value = dirs_diag }; };

// man capture directions
template<typename> struct man_capture_directions                { enum { value = dirs_up }; };

// man backwards capture
template<typename Rules> struct is_men_capture_backwards        { enum { value = man_capture_directions<Rules>::value != dirs_up }; };

// intermediate capture directions
template<int> struct turn_directions;
template<> struct turn_directions<dirs_up  >                    { enum { value = turn_up   }; };
template<> struct turn_directions<dirs_down>                    { enum { value = turn_down }; };
template<> struct turn_directions<dirs_diag>                    { enum { value = turn_diag }; };
template<> struct turn_directions<dirs_orth>                    { enum { value = turn_diag }; };
template<> struct turn_directions<dirs_all >                    { enum { value = turn_all  }; };

// king capture turn directions
template<typename Rules> struct king_turn_directions            { enum { value = turn_directions<king_capture_directions<Rules>::value>::value }; };

// man capture turn directions
template<typename Rules> struct man_turn_directions             { enum { value = turn_directions<man_capture_directions<Rules>::value>::value  }; };

// capture direction reversal
template<typename> struct is_capture_direction_reversal         { enum { value = false }; };

// king landing range after intermediate captures
template<typename Rules> struct king_capture_land               { enum { value = king_scan_range<Rules>::value }; };

// king landing range after the final capture
template<typename Rules> struct king_capture_halt               { enum { value = king_capture_land<Rules>::value }; };

// capture removal: apres-fini or en-passant
template<typename> struct capture_removal                       { enum { value = remove_af }; };

// promotion condition: apres-fini or en-passant
template<typename> struct promotion_condition                   { enum { value = promote_af }; };

//+----------------------------------------------------------------------------+
//|      Capture precedence                                                    |
//+----------------------------------------------------------------------------+

// majority capture precedence
template<typename> struct is_majority_precedence                { enum { value = false }; };

// absolute king capture precedence (applied before any majority capture precedence rule)
template<typename> struct is_absolute_king_precedence           { enum { value = false }; };

// relative king capture precedence (applied after any majority capture precedence rule)
template<typename> struct is_relative_king_precedence           { enum { value = false }; };

//+----------------------------------------------------------------------------+
//|      Drawing rules                                                         |
//+----------------------------------------------------------------------------+

// restricted repeated positions
template<typename> struct is_restricted_repetitions             { enum { value = true }; };

// maximum number of repeated positions
template<typename> struct max_repetitions                       { enum { value =  3 }; };       // (FMJD rule 6.1)

// restricted consecutive non-conversion moves
template<typename> struct is_restricted_reversible_moves        { enum { value = true }; };

// maximum allowed consecutive non-conversion moves 
template<typename> struct max_reversible_moves                  { enum { value = 50 }; };       // (FMJD rule 6.2)

// other drawing rules
template<typename> struct max_3v1_moves                         { enum { value = 32 }; };       // (FMJD rule 6.3)
template<typename> struct max_2v1_moves                         { enum { value = 10 }; };       // (FMJD rule 6.4)

//+----------------------------------------------------------------------------+
//|      Capture ambiguity                                                     |
//+----------------------------------------------------------------------------+

template<typename> struct is_check_capture_uniqueness           { enum { value = true }; };

// smallest value for which non-unique capture sequences can occur
template<typename> struct large_capture                         { enum { value = 4 }; };

template<typename Rules> struct is_ambiguous_man_capture        { enum { value = is_men_capture_backwards<Rules>::value || promotion_condition<Rules>::value }; };

}       // namespace rules
}       // namespace dctl
