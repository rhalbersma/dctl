#pragma once
#include <dctl/rules/checkers_fwd.hpp>
#include <dctl/rules/rules.hpp>
#include <dctl/rules/enum.hpp>
#include <dctl/capture/value.hpp>

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
