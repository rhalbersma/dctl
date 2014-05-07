#pragma once
#include <type_traits>                          // true_type, integral_constant
#include <dctl/rules/variants/frisian_fwd.hpp>  // Frisian
#include <dctl/rules/types/range.hpp>           // distance_N
#include <dctl/rules/types/directions.hpp>      // all
#include <dctl/rules/types/precedence.hpp>      // quality
#include <dctl/successor/value/frisian.hpp>     // Value (Frisian specialization)

namespace dctl {
namespace rules {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20

struct Frisian
{
        // main rules
        typedef range::distance_N range_king;
        typedef directions::all directions_pawn_jump;
        typedef precedence::quality precedence_jump;

        // additional rules
        using is_relative_king_jump_precedence = std::true_type;

        // drawing rules
        typedef std::integral_constant<int, 6> max_same_king_push;
        typedef std::integral_constant<int, 7> max_2Kv1K_majority_moves;
};

}       // namespace rules
}       // namespace dctl
