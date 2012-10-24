#pragma once
#include <dctl/rules/Russian_fwd.hpp>
#include <dctl/rules/Rules.hpp>
#include <dctl/rules/Enum.hpp>
#include <dctl/capture/Russian.hpp>
#include <dctl/notation/Russian.hpp>

namespace dctl {
namespace rules {

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct Russian
:
        Rules<
                Russian,
                king_range<range::distance_N>,
                pawn_jump_directions<directions::diag>,
                jump_precedence<precedence::none>,
                pawn_promotion<promotion::en_passant>
        >
{};

}       // namespace rules
}       // namespace dctl
