#pragma once
#include <dctl/rule_traits.hpp> // long_ranged_tag

namespace dctl {
namespace rules {

struct Thai
{
        static constexpr auto is_backward_pawn_jump = false;
        using king_range_category = long_ranged_tag;

        static constexpr auto initial_position_gap = 4;
        static constexpr auto is_land_behind_piece = true;
        static constexpr auto is_en_passant_jump_removal = true;
        static constexpr auto jumpsep = '-';
};

struct Dummy
{
        struct less;
};

}       // namespace rules
}       // namespace dctl
