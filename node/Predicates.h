#pragma once
#include "../utility/IntegerTypes.hpp"
#include "../utility/TemplateTricks.hpp"
#include "../rules/Rules.h"

namespace dctl {

template<typename> class Position;
class Move;

template<typename Board> BitBoard from_sq(const Position<Board>&, const Move&);
template<typename Board> BitBoard dest_sq(const Position<Board>&, const Move&);
template<typename Board> BitBoard moving_pieces(const Position<Board>&, const Move&);
template<typename Board> BitBoard moving_kings(const Position<Board>&, const Move&);
template<typename Board> BitBoard captured_pieces(const Position<Board>&, const Move&);
template<typename Board> BitBoard captured_kings(const Position<Board>&, const Move&);

template<typename Board> bool is_connected(const Position<Board>&, const Move&, const Move&);
template<typename Board> bool is_reversible(const Position<Board>&, const Move&);
template<typename Board> bool is_promotion(const Position<Board>&, const Move&);
template<typename Board> bool is_with_king(const Position<Board>&, const Move&);
template<typename Board> bool is_capture(const Position<Board>&, const Move&);

// pre-conditions for modifiers
template<typename, typename Board> bool is_pseudo_legal(const Position<Board>&, const Move&);

template<typename, typename Board> bool is_intersecting_capture(const Position<Board>&, const Move&);

// tag dispatching on capture removal
template<typename Board> bool is_intersecting_capture(const Position<Board>&, const Move&, Int2Type<rules::remove_ep>);
template<typename Board> bool is_intersecting_capture(const Position<Board>&, const Move&, Int2Type<rules::remove_af>);

template<typename> bool is_intersecting_capture(BitBoard, BitBoard);

// tag dispatching basd on capture removal
bool is_intersecting_capture(BitBoard, BitBoard, Int2Type<rules::remove_ep>);        
bool is_intersecting_capture(BitBoard, BitBoard, Int2Type<rules::remove_af>);

template<typename> bool is_intersecting_promotion(BitBoard, BitBoard);

// tag dispatching on promotion condition
bool is_intersecting_promotion(BitBoard, BitBoard, Int2Type<rules::promote_ep>);
bool is_intersecting_promotion(BitBoard, BitBoard, Int2Type<rules::promote_af>);

}       // namespace dctl

// include template definitions inside header
#include "Predicates.hpp"
