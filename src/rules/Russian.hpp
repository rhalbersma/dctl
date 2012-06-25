#pragma once
#include "Rules.hpp"
#include "Enum.hpp"
#include "../capture/Russian.hpp"

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

// alias for the Russian rules
typedef Russian Shashki;

}       // namespace rules
}       // namespace dctl

#include "../notation/Russian.hpp"
