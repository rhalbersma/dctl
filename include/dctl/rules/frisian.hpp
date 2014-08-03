#pragma once
#include <dctl/rules/frisian_fwd.hpp>           // Frisian
#include <dctl/successor/value/frisian.hpp>     // Value (Frisian specialization)

namespace dctl {
namespace rules {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20

struct Frisian
{
        // required
        static constexpr auto is_backward_pawn_jump = true;
        static constexpr auto is_jump_precedence = true;
        static constexpr auto is_long_ranged_king = true;

        // optional
        static constexpr auto is_orthogonal_jump = true;
        static constexpr auto max_same_king_push = 6;
};

}       // namespace rules
}       // namespace dctl
