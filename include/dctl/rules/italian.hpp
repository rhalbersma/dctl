#pragma once
#include <tuple>        // make_tuple

namespace dctl {
namespace rules {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
struct Italian
{
        static constexpr auto is_long_ranged_king = false;      // 4.7

        static constexpr auto is_backward_pawn_jump = false;    // 5.3(a)
        static constexpr auto is_superior_rank_jump = true;     // 5.3(b)

        static constexpr auto is_quantity_precedence = true;    // 6.6
        static constexpr auto is_modality_precedence = true;    // 6.7
        static constexpr auto is_quality_precedence = true;     // 6.8
        static constexpr auto is_ordering_precedence = true;    // 6.9

        struct precedence_tuple
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return std::make_tuple(
                                a.num_captured(),
                                a.is_with_king(),
                                a.num_captured_kings(),
                                a.piece_order()
                        );
                }
        };

        static constexpr auto max_repetitions = 4;              // 9.3(b1)
        static constexpr auto max_reversible_moves = 80;        // 10.4
};

}       // namespace rules
}       // namespace dctl
