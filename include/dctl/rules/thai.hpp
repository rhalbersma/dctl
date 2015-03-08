#pragma once
#include <dctl/rule_traits.hpp> // long_ranged_tag

namespace dctl {
namespace thai {

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;
        using king_range_category = long_ranged_tag;

        // optional
        static constexpr auto initial_gap = 4;
        static constexpr auto is_land_behind_piece = true;
        static constexpr auto is_en_passant_jump_removal = true;

        // move
        static constexpr auto jumpsep = '-';
};

}       // namespace thai
}       // namespace dctl
