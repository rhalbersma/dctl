#pragma once
#include "Checkers_fwd.hpp"
#include "Rules.hpp"
#include "Enum.hpp"
#include "../capture/Value.hpp"

namespace dctl {
namespace rules {

// http://www.usacheckers.com/rulesofcheckers.php
struct Checkers
:
        Rules<
                Checkers,
                king_range<range::distance_1>,
                pawn_jump_directions<directions::up>,
                jump_precedence<precedence::none>
        >
{};

}       // namespace rules
}       // namespace dctl
