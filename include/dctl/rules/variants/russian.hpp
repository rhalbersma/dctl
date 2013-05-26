#pragma once
#include <dctl/rules/variants/russian_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/successor/value/russian.hpp>
#include <dctl/notation/russian.hpp>

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
        typedef phase::en_passant phase_promotion;                   // 1.5.15

        // drawing rules
        typedef std::integral_constant<int, 3> max_same_king_moves;     // 1.9.1.7
};

}       // namespace rules
}       // namespace dctl
