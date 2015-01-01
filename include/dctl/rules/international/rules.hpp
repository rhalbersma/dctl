#pragma once
#include <dctl/rules/international/precedence.hpp>      // less
#include <dctl/rule_traits.hpp>

namespace dctl {
namespace international {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = true;     // 4.1
        using king_range_category = long_ranged_tag;            // 3.9

        // optional
        using jump_precedence = precedence::less;               // 4.13

        // draw
        static constexpr auto max_repetitions = 3;              // 6.1
        static constexpr auto max_reversible_moves = 50;        // 6.2
};

}       // namespace international
}       // namespace dctl
