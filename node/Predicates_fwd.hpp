#pragma once
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename Position, typename Move>
BitBoard from_sq(const Position& p, const Move& m);

template<typename Position, typename Move>
BitBoard dest_sq(const Position& p, const Move& m);

template<typename Position, typename Move>
BitBoard moving_pieces(const Position& p, const Move& m);

template<typename Position, typename Move>
BitBoard moving_kings(const Position& p, const Move& m);

template<typename Position, typename Move>
BitBoard captured_pieces(const Position& p, const Move& m);

template<typename Position, typename Move>
BitBoard captured_kings(const Position& p, const Move& m);

template<typename Position, typename Move>
bool is_connected(const Position& p, const Move& m1, const Move& m2);

template<typename Position, typename Move>
bool is_reversible(const Position& p, const Move& m);

template<typename Position, typename Move>
bool is_promotion(const Position& p, const Move& m);

template<typename Position, typename Move>
bool is_with_king(const Position& p, const Move& m);

template<typename Position, typename Move>
bool is_capture(const Position& p, const Move& m);

template<typename Position, typename Move>
bool is_pseudo_legal(const Position& p, const Move& m);

// tag dispatching on capture removal
template<typename Rules, typename Board, template<typename, typename> class Position, typename Move>
bool is_intersecting_capture(const Position<Rules, Board>& p, const Move& m);

// partial specialization for en-passant capture removal
template<typename Position, typename Move>
bool is_intersecting_capture(
        const Position& p, const Move& m, Int2Type<rules::remove_ep>
);

// partial specialization for apres-fini capture removal
template<typename Position, typename Move>
bool is_intersecting_capture(
        const Position& /* p */, const Move& /* m */, Int2Type<rules::remove_af>
);

}       // namespace dctl
