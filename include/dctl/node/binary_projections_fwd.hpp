#pragma once

namespace dctl {

template<class Position, class Move>
auto moving_pieces(Position const&, Move const&);

template<class Position, class Move>
auto moving_kings(Position const&, Move const&);

template<class Position, class Move>
auto captured_pieces(Position const&, Move const&);

template<class Position, class Move>
auto captured_kings(Position const&, Move const&);

template<class Position, class Move>
auto from_sq(Position const&, Move const&);

template<class Position, class Move>
auto dest_sq(Position const&, Move const&);

}       // namespace dctl
