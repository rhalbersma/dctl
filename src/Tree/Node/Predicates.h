#pragma once
#include "Move.h"
#include "../../utils/IntegerTypes.h"
#include "../../utils/TemplateTricks.h"
#include "../../rules/Traits.h"

namespace tree {
namespace node { 

template<typename> class Position;

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

// tag dispatching based on capture removal
template<typename Board> bool is_intersecting_capture(const Position<Board>&, const Move&, Int2Type<rules::REMOVE_1>);
template<typename Board> bool is_intersecting_capture(const Position<Board>&, const Move&, Int2Type<rules::REMOVE_N>);

template<typename> bool is_intersecting_capture(BitBoard, BitBoard);

// tag dispatching basd on capture removal
bool is_intersecting_capture(BitBoard, BitBoard, Int2Type<rules::REMOVE_1>);        
bool is_intersecting_capture(BitBoard, BitBoard, Int2Type<rules::REMOVE_N>);

template<typename> bool is_intersecting_promotion(BitBoard, BitBoard);

// tag dispatching based on promotion condition
bool is_intersecting_promotion(BitBoard, BitBoard, Int2Type<rules::PROMOTE_BR>);
bool is_intersecting_promotion(BitBoard, BitBoard, Int2Type<rules::PROMOTE_EP>);

}       // namespace node
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Predicates.hpp"
