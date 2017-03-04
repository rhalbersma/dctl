#pragma once
#include <tuple>        // make_tuple

namespace dctl {
namespace rules {
namespace block_adl {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct frisian
{
        static constexpr auto width                     = 10;
        static constexpr auto height                    = 10;
        static constexpr auto is_inverted               = false;

        static constexpr auto is_backward_pawn_jump     = true;
        static constexpr auto is_long_ranged_king       = true;

        static constexpr auto is_orthogonal_jump        = true;
        static constexpr auto max_same_king_push        = 3;

        static constexpr auto is_quantity_precedence    = true;
        static constexpr auto is_contents_precedence    = true;
        static constexpr auto is_modality_precedence    = true;

        static constexpr auto precedence = [](auto const& a) {
                auto const k = a.num_captured_kings();
                auto const n = a.num_captured_pieces();
                constexpr auto N = 65;
                auto const v = N * (n + k) - k;
                return std::make_tuple(v, a.is_with_king());
        };
};

}       // namespace block_adl

using block_adl::frisian;

}       // namespace rules
}       // namespace dctl
