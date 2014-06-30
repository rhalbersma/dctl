#pragma once
#include <dctl/rules/variants/frisian_fwd.hpp>  // Frisian
#include <dctl/successor/value/frisian.hpp>     // Value (Frisian specialization)

namespace dctl {
namespace rules {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20

struct Frisian
{
        // main rules
        static constexpr auto is_long_ranged_king = true;
        static constexpr auto is_backward_pawn_jump = true;
        static constexpr auto is_jump_precedence = true;

        // additional rules
        static constexpr auto is_orthogonal_jump = true;
        static constexpr auto is_relative_king_jump_precedence = true;

        // drawing rules
        static constexpr auto max_same_king_push = 6;
        static constexpr auto max_2Kv1K_majority_moves = 7;
};

}       // namespace rules
}       // namespace dctl
