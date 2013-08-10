#pragma once
#include <dctl/utility/int.hpp>

namespace dctl {

template<class Position, class Move>
BitBoard moving_pieces(Position const&, Move const&);

template<class Position, class Move>
BitBoard moving_kings(Position const&, Move const&);

template<class Position, class Move>
BitBoard captured_pieces(Position const&, Move const&);

template<class Position, class Move>
BitBoard captured_kings(Position const&, Move const&);

template<class Position, class Move>
BitBoard from_sq(Position const&, Move const&);

template<class Position, class Move>
BitBoard dest_sq(Position const&, Move const&);

}       // namespace dctl
