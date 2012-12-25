#pragma once
#include <type_traits>
#include <dctl/rules/frisian_fwd.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/capture/frisian.hpp>

namespace dctl {
namespace rules {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20

struct Frisian
{
        // main rules
        typedef range::distance_N king_range;
        typedef directions::all pawn_jump_directions;
        typedef precedence::quality jump_precedence;

        // additional rules
        typedef std::true_type is_relative_king_precedence;

        // drawing rules
        typedef std::integral_constant<int, 6> max_same_king_moves;
        typedef std::integral_constant<int, 7> max_2Kv1K_majority_moves;
};

}       // namespace rules
}       // namespace dctl
