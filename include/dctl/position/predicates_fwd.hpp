#pragma once

namespace dctl {

template<class Position, class Move>
bool is_connected(Position const&, Move const&, Move const&);

template<class Position, class Move>
bool is_promotion(Position const&, Move const& m);

template<class Position, class Move>
bool is_with_king(Position const&, Move const& m);

template<class Position, class Move>
bool is_capture(Position const&, Move const& m);

template<class Position, class Move>
bool is_reversible(Position const&, Move const& m);

template<class Position, class Move>
bool is_intersecting_capture(Position const&, Move const& m);

template<class Position, class Move>
bool is_pseudo_legal(Position const&, Move const& m);

}       // namespace dctl
