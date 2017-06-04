#pragma once

namespace dctl {
namespace core {
namespace block_adl {

// http://www.usacheckers.com/rulesofcheckers.php
struct checkers
{
        // THE DRAUGHTBOARD AND ITS ARRANGEMENT
        static constexpr auto width                     = 8;            // 1.1
        static constexpr auto height                    = 8;            // 1.1
        static constexpr auto is_inverted               = false;        // 1.4
        static constexpr auto is_algebraic_notation     = false;        // 1.5

        // DRAUGHT PIECES AND THEIR ARRANGEMENT
        static constexpr auto initial_position_gap      = 2;            // 1.11

        // Ordinary Move Of A King
        static constexpr auto is_long_ranged_king       = false;        // 1.17

        // Capturing Move Of A Man
        static constexpr auto is_backward_pawn_jump     = false;        // 1.18
        static constexpr auto is_superior_rank_jump     = false;        // 1.18

        // Capturing In General
        static constexpr auto is_passing_promotion      = false;        // 1.19
        static constexpr auto is_passing_capture        = false;        // 1.19
        static constexpr auto is_quantity_precedence    = false;        // 1.20

        // Definition of a Draw
        static constexpr auto max_repetitions           = 3;            // 1.32.1
        static constexpr auto max_reversible_moves      = 80;           // 1.32.2
};

}       // namespace block_adl

using block_adl::checkers;

}       // namespace core
}       // namespace dctl
