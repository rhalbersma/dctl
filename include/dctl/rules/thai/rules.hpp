#pragma once
#include <dctl/rule_traits.hpp>

namespace dctl {
namespace thai {

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;
        using king_range_category = long_ranged_tag;

        // optional
        static constexpr auto initial_gap = 4;
        static constexpr auto is_directly_land_after_piece = true;
        static constexpr auto is_en_passant_jump_removal = true;
};

}       // namespace thai
}       // namespace dctl
