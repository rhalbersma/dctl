#pragma once
#include <type_traits>                          // integral_constant
#include <dctl/rules/variants/russian_fwd.hpp>  // Russian
#include <dctl/rules/types/directions.hpp>      // diag
#include <dctl/rules/types/precedence.hpp>      // none
#include <dctl/successor/value/russian.hpp>     // Value (Russian specialization)

namespace dctl {
namespace rules {

// http://www.shashist.ru/kodeks/kodeks2004.doc

struct Russian
{
        // main rules
        using is_long_ranged_king = std::true_type;                     // 1.4.5
        using is_backward_pawn_jump = std::true_type;                   // 1.5.3
        using precedence_jump = precedence::none;                       // 1.5.14

        // additional rules
        using is_en_passant_promotion = std::true_type;                 // 1.5.15
};

}       // namespace rules
}       // namespace dctl
