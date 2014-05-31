#pragma once
#include <dctl/rules/variants/frisian_fwd.hpp>  // Frisian
#include <dctl/rules/types/directions.hpp>      // all
#include <dctl/rules/types/precedence.hpp>      // quality
#include <dctl/successor/value/frisian.hpp>     // Value (Frisian specialization)
#include <type_traits>                          // true_type, integral_constant

namespace dctl {
namespace rules {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20

struct Frisian
{
        // main rules
        using is_long_ranged_king = std::true_type;
        using is_backward_pawn_jump = std::true_type;
        using is_orthogonal_jump = std::true_type;
        using precedence_jump = precedence::quality;

        // additional rules
        using is_relative_king_jump_precedence = std::true_type;

        // drawing rules
        using max_same_king_push        = std::integral_constant<int, 6>;
        using max_2Kv1K_majority_moves  = std::integral_constant<int, 7>;
};

}       // namespace rules
}       // namespace dctl
