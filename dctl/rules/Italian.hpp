#pragma once
#include <dctl/rules/Italian_fwd.hpp>
#include <dctl/rules/Rules.hpp>
#include <dctl/rules/Enum.hpp>
#include <dctl/capture/Italian.hpp>

namespace dctl {
namespace rules {

// http://www.fid.it/regolamenti/2008/RegTec_CAPO_I.pdf
struct Italian
:
        Rules<
                Italian,
                king_range<range::distance_1>,
                pawn_jump_directions<directions::up>,
                jump_precedence<precedence::quality>,
                is_pawns_jump_kings<boost::mpl::false_>,
                is_relative_king_precedence<boost::mpl::true_>
        >
{};

}       // namespace rules
}       // namespace dctl
