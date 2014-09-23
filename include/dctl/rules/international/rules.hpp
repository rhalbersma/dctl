#pragma once
#include <dctl/rules/international/precedence.hpp>      // less

namespace dctl {
namespace international {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = true;     // 4.1
        static constexpr auto is_long_ranged_king = true;       // 3.9

        using jump_precedence = precedence::less;               // 4.13

        // draw
        static constexpr auto max_repetitions = 3;              // 6.1
        static constexpr auto max_reversible_moves = 50;        // 6.2
};

}       // namespace international
}       // namespace dctl
