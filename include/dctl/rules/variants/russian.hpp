#pragma once
#include <type_traits>                          // integral_constant
#include <dctl/rules/variants/russian_fwd.hpp>  // Russian
#include <dctl/rules/types/range.hpp>           // distance_N
#include <dctl/rules/types/directions.hpp>      // diag
#include <dctl/rules/types/precedence.hpp>      // none
#include <dctl/rules/types/phase.hpp>           // en_passant
#include <dctl/successor/value/russian.hpp>     // Value (Russian specialization)

namespace dctl {
namespace rules {

// http://www.shashist.ru/kodeks/kodeks2004.doc

struct Russian
{
        // main rules
        typedef range::distance_N range_king;                           // 1.4.5
        typedef directions::diag directions_pawn_jump;                  // 1.5.3
        typedef precedence::none precedence_jump;                       // 1.5.14

        // additional rules
        using promotion_phase = en_passant;                             // 1.5.15
};

}       // namespace rules
}       // namespace dctl
