#pragma once
#include <dctl/rules/variants/italian_fwd.hpp>  // Italian
#include <dctl/rules/types/directions.hpp>      // up
#include <dctl/rules/types/precedence.hpp>      // quality
#include <dctl/rules/types.hpp>
#include <dctl/successor/value/italian.hpp>     // Value (Italian specialization)
#include <type_traits>                          // false_type, true_type, integral_constant

namespace dctl {
namespace rules {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

struct Italian
{
        // main rules
        using is_long_ranged_king = std::false_type;                            // 4.7
        using is_backward_pawn_jump = std::false_type;                          // 5.3(a)
        using precedence_jump = precedence::quality;                            // 6.1 - 6.10

        // additional rules
        template<class /* Attacker */, class /* Victim */, class = void>
        struct can_jump: std::true_type {};

        template<class _>
        struct can_jump<pieces::pawn, pieces::king, _>: std::false_type {};     // 5.3(b)

        using is_relative_king_jump_precedence = std::true_type;                // 6.7

        // drawing rules
        using max_repetitions = std::integral_constant<int,  4>;                // 9.3(b1)
        using max_reversible_moves = std::integral_constant<int, 80>;           // 10.4
};

}       // namespace rules
}       // namespace dctl
