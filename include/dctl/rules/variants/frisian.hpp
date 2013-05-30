#pragma once
#include <type_traits>
#include <dctl/rules/variants/frisian_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/successor/value/frisian.hpp>

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
        typedef std::true_type precedence_is_relative_king;

        // drawing rules
        typedef std::integral_constant<int, 6> max_same_king_moves;
        typedef std::integral_constant<int, 7> max_2Kv1K_majority_moves;
};

}       // namespace rules
}       // namespace dctl
