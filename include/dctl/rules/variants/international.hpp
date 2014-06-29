#pragma once
#include <dctl/rules/variants/international_fwd.hpp>    // International
#include <dctl/rules/types/precedence.hpp>              // quantity
#include <dctl/successor/value/international.hpp>       // Value (International specialization)
#include <type_traits>                                  // integral_constant

namespace dctl {
namespace rules {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc

struct International
{
        // main rules
        static constexpr auto is_long_ranged_king = true;               // 3.9
        static constexpr auto is_backward_pawn_jump = true;             // 4.1
        using precedence_jump = precedence::quantity;                   // 4.13

        // drawing rules
        using max_repetitions = std::integral_constant<int,  3>;        // 6.1
        using max_reversible_moves = std::integral_constant<int, 50>;   // 6.2
        using max_3v1_moves = std::integral_constant<int, 32>;          // 6.3
        using max_2v1_moves = std::integral_constant<int, 10>;          // 6.4
};

}       // namespace rules
}       // namespace dctl
