#pragma once
#include <dctl/piece.hpp>       // king
#include <tuple>                // make_tuple

namespace dctl {
namespace rules {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20

struct Frisian
{
        static constexpr auto is_backward_pawn_jump = true;
        static constexpr auto is_long_ranged_king = true;

        static constexpr auto is_orthogonal_jump = true;
        static constexpr auto max_same_king_push = 3;

        static constexpr auto is_quantity = true;
        static constexpr auto is_quality = true;
        static constexpr auto is_modality = true;

        struct precedence
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        auto const k = a.num_captured(Piece::king);
                        auto const n = a.num_captured();
                        constexpr auto N = 65;
                        auto const v = N * (n + k) - k;
                        return std::make_tuple(
                                v, a.is_with(Piece::king)
                        );
                }
        };
};

}       // namespace rules
}       // namespace dctl
