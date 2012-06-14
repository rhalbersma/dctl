#pragma once
#include "Rules.hpp"
#include "Enum.hpp"

namespace dctl {
namespace rules {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc
struct International
: 
        Rules< 
                king_range<range::distance_N>,
                pawn_jump_directions<directions::diag>,
                jump_precedence<precedence::quantity>
        > 
{};

// aliases for the international rules
typedef International Brazilian;        //  8x8  board
typedef International Polish;           // 10x10 board
typedef International Canadian;         // 12x12 board

}       // namespace rules
}       // namespace dctl

#include "../capture/International.hpp"
