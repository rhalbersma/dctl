#pragma once
#include "Enum.h"
#include "Variants.h"

namespace dctl {
namespace rules {

//+----------------------------------------------------------------------------+
//|      Move mechanics                                                        |
//+----------------------------------------------------------------------------+

// king range
template<typename> struct is_long_king_range                    { enum { value = RANGE_N }; };

// restricted consecutive moves with the same king
template<typename> struct is_restricted_same_king_moves         { enum { value = false }; };

// maximum consecutive moves with the same king
template<typename> struct max_same_king_moves                   { enum { value = 6 }; };

// promotion condition: on the back row or en-passant
template<typename> struct promotion_condition                   { enum { value = PROMOTE_BR }; };

//+----------------------------------------------------------------------------+
//|      Capture mechanics                                                     |
//+----------------------------------------------------------------------------+

// king capture directions
template<typename> struct king_capture_directions               { enum { value = DIRS_DIAG }; };

// man capture directions
template<typename> struct man_capture_directions                { enum { value = DIRS_DIAG }; };

// man backwards capture
template<typename Rules> struct is_men_capture_backwards        { enum { value = man_capture_directions<Rules>::value != DIRS_UP }; };

// relation between initial and intermediate capture directions
template<int> struct scan_directions;
template<> struct scan_directions<DIRS_UP  >                    { enum { value = SCAN_UP   }; };
template<> struct scan_directions<DIRS_DOWN>                    { enum { value = SCAN_DOWN }; };
template<> struct scan_directions<DIRS_DIAG>                    { enum { value = SCAN_SIDE }; };
template<> struct scan_directions<DIRS_ORTH>                    { enum { value = SCAN_SIDE }; };
template<> struct scan_directions<DIRS_ALL >                    { enum { value = SCAN_ALL  }; };

// king capture scan directions
template<typename Rules> struct king_scan_directions            { enum { value = scan_directions<king_capture_directions<Rules>::value>::value }; };

// man capture scan directions
template<typename Rules> struct man_scan_directions             { enum { value = scan_directions<man_capture_directions<Rules>::value>::value  }; };

// capture direction reversal
template<typename> struct is_capture_direction_reversal         { enum { value = false }; };

// king halt after final capture
template<typename Rules> struct king_capture_halt               { enum { value = is_long_king_range<Rules>::value }; };

// capture removal: one by one or all at once
template<typename> struct capture_removal                       { enum { value = REMOVE_N }; };

// men can capture kings
template<typename> struct is_men_capture_kings                  { enum { value = true  }; };

//+----------------------------------------------------------------------------+
//|      Capture precedence                                                    |
//+----------------------------------------------------------------------------+

// majority capture precedence
template<typename> struct is_majority_precedence                { enum { value = false }; };

// absolute king capture precedence (applied before any majority capture precedene rule)
template<typename> struct is_absolute_king_precedence           { enum { value = false }; };

// relative king capture precedence (applied after any majority capture precedenc rule)
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
//|      Implementation of move generation                                     |
//+----------------------------------------------------------------------------+

template<typename> struct is_check_capture_uniqueness           { enum { value = true }; };

// smallest value for which non-unique capture sequences can occur
template<typename> struct large_capture                         { enum { value = 4 }; };

template<typename Rules> struct is_ambiguous_man_capture        { enum { value = is_men_capture_backwards<Rules>::value || promotion_condition<Rules>::value }; };

}       // namespace rules
}       // namespace dctl
