#pragma once
#include <type_traits>
#include <dctl/rules/variants/italian_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/successor/value/italian.hpp>

namespace dctl {
namespace rules {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf

struct Italian
{
        // main rules
        typedef range::distance_1 range_king;                           // 4.7
        typedef directions::up directions_pawn_jump;                    // 5.3(a)
        typedef precedence::quality precedence_jump;                    // 6.1 - 6.10

        // additional rules
        typedef std::false_type is_pawns_jump_kings;                    // 5.3(b)
        typedef std::true_type is_relative_king_precedence;             // 6.7

        // drawing rules
        typedef std::integral_constant<int,  4> max_repetitions;        // 9.3(b1)
        typedef std::integral_constant<int, 80> max_reversible_moves;   // 10.4
};

}       // namespace rules
}       // namespace dctl
