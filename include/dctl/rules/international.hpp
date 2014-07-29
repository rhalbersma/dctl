#pragma once
#include <dctl/rules/international_fwd.hpp>             // International
#include <dctl/successor/value/international.hpp>       // Value (International specialization)

namespace dctl {
namespace rules {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc

struct International
{
        // main rules
        static constexpr auto is_long_ranged_king = true;       // 3.9
        static constexpr auto is_backward_pawn_jump = true;     // 4.1
        static constexpr auto is_jump_precedence = true;        // 4.13

        // drawing rules
        static constexpr auto max_repetitions = 3;              // 6.1
        static constexpr auto max_reversible_moves = 50;        // 6.2
        static constexpr auto max_3v1_moves = 32;               // 6.3
        static constexpr auto max_2v1_moves = 10;               // 6.4
};

}       // namespace rules
}       // namespace dctl
