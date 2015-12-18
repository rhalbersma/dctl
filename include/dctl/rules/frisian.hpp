#pragma once
#include <tuple>        // make_tuple

namespace dctl {
namespace rules {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct Frisian
{
        static constexpr auto is_backward_pawn_jump = true;
        static constexpr auto is_long_ranged_king = true;

        static constexpr auto is_orthogonal_jump = true;
        static constexpr auto max_same_king_push = 3;

        static constexpr auto is_quantity_precedence = true;
        static constexpr auto is_quality_precedence = true;
        static constexpr auto is_modality_precedence = true;

        struct precedence_tuple
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        auto const k = a.num_captured_kings();
                        auto const n = a.num_captured();
                        constexpr auto N = 65;
                        auto const v = N * (n + k) - k;
                        return std::make_tuple(
                                v, a.is_with_king()
                        );
                }
        };
};

}       // namespace rules
}       // namespace dctl
