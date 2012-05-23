#pragma once
#include "Rules.hpp"

namespace dctl {
namespace rules {

// http://www.usacheckers.com/rulesofcheckers.php
struct Checkers
: 
        Rules<
                king_range<range::distance_1>,
                pawn_jump_directions<directions::up>,
                jump_precedence<precedence::none>
        > 
{};

// aliases for the rules of checkers
typedef Checkers English;
typedef Checkers American;

}       // namespace rules
}       // namespace dctl
