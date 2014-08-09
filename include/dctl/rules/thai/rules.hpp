#pragma once
#include <dctl/successor/value.hpp>     // Value

namespace dctl {
namespace thai {

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;
        static constexpr auto is_jump_precedence = false;
        static constexpr auto is_long_ranged_king = true;

        template<class Move>
        using value_type = successor::Value<Move>;

        // optional
        static constexpr auto initial_gap = 4;
        static constexpr auto is_directly_land_after_piece = true;
        static constexpr auto is_en_passant_jump_removal = true;
};

}       // namespace thai
}       // namespace dctl
