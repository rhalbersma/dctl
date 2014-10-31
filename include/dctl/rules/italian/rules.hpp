#pragma once
#include <dctl/rules/italian/precedence.hpp>    // less

namespace dctl {
namespace italian {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;    // 5.3(a)
        static constexpr auto is_long_ranged_king = false;      // 4.7

        // optional
        using jump_precedence = precedence::less;               // 6.1 - 6.10
        static constexpr auto is_pawn_jump_king = false;        // 5.3(b)

        // draw
        static constexpr auto max_repetitions = 4;              // 9.3(b1)
        static constexpr auto max_reversible_moves = 80;        // 10.4
};

}       // namespace italian
}       // namespace dctl
