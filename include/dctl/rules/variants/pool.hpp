#pragma once
#include <type_traits>                          // integral_constant
#include <dctl/rules/variants/pool_fwd.hpp>     // Pool
#include <dctl/rules/types/directions.hpp>      // diag
#include <dctl/rules/types/precedence.hpp>      // none
#include <dctl/successor/value.hpp>             // Value (primary template)

namespace dctl {
namespace rules {

// http://americanpoolcheckers.us/americanpoolcheckers/index.php/history/apca-tournament-rules-of-play

struct Pool
{
        // main rules
        using is_long_ranged_king = std::true_type;                             // 15
        using is_backward_pawn_jump = std::true_type;                           // 13
        using precedence_jump = precedence::none;                               // 20

        // drawing rules
        using max_repetitions = std::integral_constant<int,  3>;                // 26
        using max_reversible_moves = std::integral_constant<int, 30>;           // 26
        using max_3Kv1K_minority_moves = std::integral_constant<int, 13>;       // 27
        using max_LLv1K_majority_moves = std::integral_constant<int,  5>;       // 28
};

}       // namespace rules
}       // namespace dctl
