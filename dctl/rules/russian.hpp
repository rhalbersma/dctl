#pragma once
#include <dctl/rules/russian_fwd.hpp>
#include <dctl/rules/rules.hpp>
#include <dctl/rules/enum.hpp>
#include <dctl/capture/russian.hpp>
#include <dctl/notation/russian.hpp>

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
