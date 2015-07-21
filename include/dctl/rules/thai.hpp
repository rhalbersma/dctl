#pragma once
#include <dctl/rule_traits.hpp> // long_ranged_tag, passing_capture_tag

namespace dctl {
namespace rules {

struct Thai
{
        static constexpr auto is_backward_pawn_jump = false;
        using king_range_category = long_ranged_tag;

        static constexpr auto initial_position_gap = 4;
        static constexpr auto is_land_behind_piece = true;
        using capture_category = passing_capture_tag;
        static constexpr auto jumpsep = '-';
};

}       // namespace rules
}       // namespace dctl
