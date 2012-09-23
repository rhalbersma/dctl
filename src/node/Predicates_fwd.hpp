#pragma once

namespace dctl {

template<typename Position, typename Move>
BitBoard from_sq(Position const& p, Move const& m);

template<typename Position, typename Move>
BitBoard dest_sq(Position const& p, Move const& m);

template<typename Position, typename Move>
BitBoard moving_pieces(Position const& p, Move const& m);

template<typename Position, typename Move>
BitBoard moving_kings(Position const& p, Move const& m);

template<typename Position, typename Move>
BitBoard captured_pieces(Position const& p, Move const& m);

template<typename Position, typename Move>
BitBoard captured_kings(Position const& p, Move const& m);

template<typename Position, typename Move>
bool is_connected(Position const& p, Move const& m1, Move const& m2);

template<typename Position, typename Move>
bool is_reversible(Position const& p, Move const& m);

template<typename Position, typename Move>
bool is_promotion(Position const& p, Move const& m);

template<typename Position, typename Move>
bool is_with_king(Position const& p, Move const& m);

template<typename Position, typename Move>
bool is_capture(Position const& p, Move const& m);

template<typename Position, typename Move>
bool is_pseudo_legal(Position const& p, Move const& m);

template<typename Position, typename Move>
bool is_intersecting_capture(Position const& p, Move const& m);

}       // namespace dctl
