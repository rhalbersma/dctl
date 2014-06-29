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
        static constexpr auto is_long_ranged_king     = true;   // 1.4.5
        static constexpr auto is_backward_pawn_jump   = true;   // 1.5.3
        using precedence_jump = precedence::none;               // 1.5.14

        // drawing rules
        static constexpr auto is_en_passant_promotion = true;   // 1.5.15
};

}       // namespace rules
}       // namespace dctl
