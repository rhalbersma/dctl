#pragma once
#include <type_traits>                          // false_type, true_type, integral_constant
#include <dctl/rules/variants/italian_fwd.hpp>  // Italian
#include <dctl/rules/types/range.hpp>           // distance_1
#include <dctl/rules/types/directions.hpp>      // up
#include <dctl/rules/types/precedence.hpp>      // quality
#include <dctl/rules/types.hpp>
#include <dctl/successor/value/italian.hpp>     // Value (Italian specialization)

namespace dctl {
namespace rules {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

struct Italian
{
        // main rules
        typedef range::distance_1 range_king;                                   // 4.7
        typedef directions::up directions_pawn_jump;                            // 5.3(a)
        typedef precedence::quality precedence_jump;                            // 6.1 - 6.10

        // additional rules
        template<class /* Attacker */, class /* Victim */, class = void>
        struct can_jump: std::true_type {};

        template<class _>
        struct can_jump<pieces::pawn, pieces::king, _>: std::false_type {};     // 5.3(b)

        typedef std::true_type precedence_is_relative_king;                     // 6.7

        // drawing rules
        typedef std::integral_constant<int,  4> max_repetitions;                // 9.3(b1)
        typedef std::integral_constant<int, 80> max_reversible_moves;           // 10.4
};

}       // namespace rules
}       // namespace dctl
