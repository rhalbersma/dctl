#pragma once

namespace dctl {
namespace rules {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc
struct international
{
        // 2. The material
        static constexpr auto initial_gap = 2;                  // 2.8

        // 3. The movement of the piece
        static constexpr auto is_long_ranged_king = true;       // 3.9

        // 4. The capture
        static constexpr auto is_backward_pawn_jump = true;     // 4.1
        static constexpr auto is_superior_rank_jump = false;    // 4.2 (implied by 3.1)
        static constexpr auto is_passing_capture = false;       // 4.11
        static constexpr auto is_quantity_precedence = true;    // 4.13
        static constexpr auto is_quality_precedence = false;    // 4.13
        static constexpr auto is_modality_precedence = false;   // 4.14

        struct tuple_type
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return a.num_captured_pieces();
                }
        };

        static constexpr auto is_passing_promotion = false;     // 4.15 (and 3.5)

        // 6. The draw
        static constexpr auto max_repetitions = 3;              // 6.1
        static constexpr auto max_reversible_moves = 50;        // 6.2

        // 8. Notation
        static constexpr auto is_algebraic_notation = false;    // 8.1 (and 2.6)
        static constexpr auto pushsep = '-';                    // 8.2.2
        static constexpr auto jumpsep = 'x';                    // 8.2.3
};

using Brazilian  = international;
using Philippine = international;
using canadian   = international;
using srilankan  = international;

}       // namespace rules
}       // namespace dctl
