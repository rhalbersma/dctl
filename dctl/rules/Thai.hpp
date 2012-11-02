#pragma once
#include <boost/mpl/int.hpp>
#include <dctl/rules/thai_fwd.hpp>
#include <dctl/rules/rules.hpp>
#include <dctl/rules/enum.hpp>
#include <dctl/capture/value.hpp>
#include <dctl/notation/thai.hpp>

namespace dctl {
namespace rules {

struct Thai
:
        Rules<
                Thai,
                king_range<range::distance_N>,
                pawn_jump_directions<directions::up>,
                jump_precedence<precedence::none>,
                land_range<range::distance_1>,
                is_jump_direction_reversal<boost::mpl::true_>,
                jump_removal<removal::en_passant>,
                initial_dmz< boost::mpl::int_<4> >
        >
{};

}       // namespace rules
}       // namespace dctl
