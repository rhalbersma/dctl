#pragma once
#include "Position_fwd.hpp"
#include "Move_fwd.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename Board>
BitBoard from_sq(const Position<Board>& p, const Move& m);

template<typename Board>
BitBoard dest_sq(const Position<Board>& p, const Move& m);

template<typename Board>
BitBoard moving_pieces(const Position<Board>& p, const Move& m);

template<typename Board>
BitBoard moving_kings(const Position<Board>& p, const Move& m);

template<typename Board>
BitBoard captured_pieces(const Position<Board>& p, const Move& m);

template<typename Board>
BitBoard captured_kings(const Position<Board>& p, const Move& m);

template<typename Board>
bool is_connected(const Position<Board>& p, const Move& m1, const Move& m2);

template<typename Board>
bool is_reversible(const Position<Board>& p, const Move& m);

template<typename Board>
bool is_promotion(const Position<Board>& p, const Move& m);

template<typename Board>
bool is_with_king(const Position<Board>& p, const Move& m);

template<typename Board>
bool is_capture(const Position<Board>& p, const Move& m);

template<typename Rules, typename Board>
bool is_pseudo_legal(const Position<Board>& p, const Move& m);

// tag dispatching on capture removal
template<typename Rules, typename Board>
bool is_intersecting_capture(const Position<Board>& p, const Move& m);

// partial specialization for en-passant capture removal
template<typename Board>
bool is_intersecting_capture(
        const Position<Board>& p, const Move& m, Int2Type<rules::remove_ep>
);

// partial specialization for apres-fini capture removal
template<typename Board>
bool is_intersecting_capture(
        const Position<Board>& /* p */, const Move& /* m */, Int2Type<rules::remove_af>
);

}       // namespace dctl
