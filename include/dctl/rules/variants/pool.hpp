#pragma once
#include <type_traits>                          // integral_constant
#include <dctl/rules/variants/pool_fwd.hpp>     // Pool
#include <dctl/rules/types/range.hpp>           // distance_N
#include <dctl/rules/types/directions.hpp>      // diag
#include <dctl/rules/types/precedence.hpp>      // none
#include <dctl/successor/value.hpp>             // Value (primary template)

namespace dctl {
namespace rules {

// http://americanpoolcheckers.us/americanpoolcheckers/index.php/history/apca-tournament-rules-of-play

struct Pool
{
        // main rules
        typedef range::distance_N range_king;                                   // 15
        typedef directions::diag directions_pawn_jump;                          // 13
        typedef precedence::none precedence_jump;                               // 20

        // drawing rules
        typedef std::integral_constant<int,  3> max_repetitions;                // 26
        typedef std::integral_constant<int, 30> max_reversible_moves;           // 26
        typedef std::integral_constant<int, 13> max_3Kv1K_minority_moves;       // 27
        typedef std::integral_constant<int,  5> max_LLv1K_majority_moves;       // 28
};

}       // namespace rules
}       // namespace dctl
