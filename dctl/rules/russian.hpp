#pragma once
#include <dctl/rules/russian_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/capture/russian.hpp>
#include <dctl/notation/russian.hpp>

namespace dctl {
namespace rules {

// http://www.shashist.ru/kodeks/kodeks2004.doc

struct Russian
{
        // main rules
        typedef range::distance_N king_range;                           // 1.4.5
        typedef directions::diag pawn_jump_directions;                  // 1.5.3
        typedef precedence::none jump_precedence;                       // 1.5.14

        // additional rules
        typedef promotion::en_passant pawn_promotion;                   // 1.5.15

        // drawing rules
        typedef std::integral_constant<int, 3> max_same_king_moves;     // 1.9.1.7
};

}       // namespace rules
}       // namespace dctl
