#pragma once

namespace dctl {
namespace rules {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc

struct International
{
        static constexpr auto is_long_ranged_king = true;       // 3.9
        static constexpr auto is_backward_pawn_jump = true;     // 4.1

        static constexpr auto is_quantity = true;               // 4.13

        struct precedence
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return a.num_captured();
                }
        };

        static constexpr auto max_repetitions = 3;              // 6.1
        static constexpr auto max_reversible_moves = 50;        // 6.2
};

using Brazilian  = International;
using Philippine = International;
using Canadian   = International;
using SriLankan  = International;

}       // namespace rules
}       // namespace dctl
