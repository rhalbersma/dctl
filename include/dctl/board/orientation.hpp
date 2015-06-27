#pragma once
#include <dctl/angle.hpp>       // _deg, inverse, rotate
#include <dctl/player.hpp>       // Player, white

namespace dctl {

template<class Board, Player ToMove, bool Reverse = false>
constexpr auto orientation_v = rotate(inverse(Board::orientation), ((ToMove == Player::white) ^ Reverse) ? 0_deg : 180_deg);

}       // namespace dctl
