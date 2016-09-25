#pragma once
#include <tuple>        // make_tuple

namespace dctl {
namespace rules {
namespace block_adl {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
struct Italian
{
        // Art. 2 - Il materiale
        static constexpr auto width                     = 8;            // 2.1
        static constexpr auto height                    = 8;            // 2.1
        static constexpr auto is_inverted               = true;         // 2.3

        // Art. 4 - Gli spostamenti semplici
        static constexpr auto is_long_ranged_king       = false;        // 4.7

        // Art. 5 - Le prese
        static constexpr auto is_backward_pawn_jump     = false;        // 5.3(a)
        static constexpr auto is_superior_rank_jump     = true;         // 5.3(b)

        // Art. 6 - Le regole di presa
        static constexpr auto is_quantity_precedence    = true;         // 6.6
        static constexpr auto is_modality_precedence    = true;         // 6.7
        static constexpr auto is_quality_precedence     = true;         // 6.8
        static constexpr auto is_ordering_precedence    = true;         // 6.9

        struct tuple_type
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return std::make_tuple(
                                a.num_captured_pieces(),
                                a.is_with_king(),
                                a.num_captured_kings(),
                                a.piece_order()
                        );
                }
        };

        static constexpr auto max_repetitions           = 4;            // 9.3(b1)
        static constexpr auto max_reversible_moves      = 80;           // 10.4
};

}       // namespace block_adl

using block_adl::Italian;

}       // namespace rules
}       // namespace dctl
