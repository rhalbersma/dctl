#pragma once
#include <dctl/rules/variants/italian_fwd.hpp>  // Italian
#include <dctl/rules/types.hpp>
#include <dctl/successor/value/italian.hpp>     // Value (Italian specialization)
#include <type_traits>                          // true_type, false_type

namespace dctl {
namespace rules {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

struct Italian
{
        // main rules
        static constexpr auto is_long_ranged_king = false;                      // 4.7
        static constexpr auto is_backward_pawn_jump = false;                    // 5.3(a)
        static constexpr auto is_jump_precedence = true;                        // 6.1 - 6.10

        // additional rules
        static constexpr auto is_pawn_jump_king = false;                        // 5.3(b)
        static constexpr auto is_relative_king_jump_precedence = true;          // 6.7

        // drawing rules
        static constexpr auto max_repetitions = 4;                              // 9.3(b1)
        static constexpr auto max_reversible_moves = 80;                        // 10.4
};

}       // namespace rules
}       // namespace dctl
