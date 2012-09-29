#pragma once
#include "Frisian_fwd.hpp"
#include "Rules.hpp"
#include "Enum.hpp"
#include "../capture/Frisian.hpp"

namespace dctl {
namespace rules {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct Frisian
:
        Rules<
                Frisian,
                king_range<range::distance_N>,
                pawn_jump_directions<directions::all>,               
                jump_precedence<precedence::quality>,
                is_restricted_same_king_moves<boost::mpl::true_>,
                max_same_king_moves< boost::mpl::int_<6> >,
                is_relative_king_precedence<boost::mpl::true_>
        >
{};

}       // namespace rules
}       // namespace dctl
