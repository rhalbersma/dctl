#pragma once

namespace dctl::core {
namespace block_adl {

struct thai
{
        static constexpr auto is_long_ranged_king = true;
        static constexpr auto is_backward_pawn_jump = false;

        static constexpr auto initial_position_gap = 4;
        static constexpr auto is_land_behind_piece = true;
        static constexpr auto is_passing_capture = true;
        static constexpr auto jumpsep = '-';
};

}       // namespace block_adl

using block_adl::thai;

}       // namespace dctl::core
