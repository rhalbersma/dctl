#pragma once
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename> struct Position;
struct Move;

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

// tag dispatching on capture removal
template<typename Rules>
bool is_intersecting_capture(BitBoard delta, BitBoard captured_pieces);

// specialization for en-passant capture removal (Thai draughts)
bool is_intersecting_capture(
        BitBoard delta, BitBoard captured_pieces, Int2Type<rules::remove_ep>
);

// specialization for apres-fini capture removal
bool is_intersecting_capture(
        BitBoard /* delta */, BitBoard /* captured_pieces */, Int2Type<rules::remove_af>
);

// tag dispatching on promotion condition
template<typename Rules>
bool is_intersecting_promotion(BitBoard promotion, BitBoard delta);

// specialization for en-passant promotion (Russian draughts)
bool is_intersecting_promotion(
        BitBoard promotion, BitBoard delta, Int2Type<rules::promote_ep>
);

// specialization for apres-fini promotion
bool is_intersecting_promotion(
        BitBoard /* promotion */, BitBoard /* delta */, Int2Type<rules::promote_af>
);

}       // namespace dctl
