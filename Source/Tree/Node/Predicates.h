#pragma once
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"
#include "../../Variants/Rules.h"

namespace tree {
namespace node { 

template<typename> class Position;
class Pieces;

template<typename Board> BitBoard from_sq(const Position<Board>&, const Pieces&);
template<typename Board> BitBoard dest_sq(const Position<Board>&, const Pieces&);
template<typename Board> BitBoard moving_pieces(const Position<Board>&, const Pieces&);
template<typename Board> BitBoard moving_kings(const Position<Board>&, const Pieces&);
template<typename Board> BitBoard captured_pieces(const Position<Board>&, const Pieces&);
template<typename Board> BitBoard captured_kings(const Position<Board>&, const Pieces&);

template<typename Board> bool is_connected(const Position<Board>&, const Pieces&, const Pieces&);
template<typename Board> bool is_non_conversion(const Position<Board>&, const Pieces&);
template<typename Board> bool is_promotion(const Position<Board>&, const Pieces&);
template<typename Board> bool is_with_king(const Position<Board>&, const Pieces&);
template<typename Board> bool is_capture(const Position<Board>&, const Pieces&);

// pre-conditions for modifiers
template<typename, typename Board> bool is_pseudo_legal(const Position<Board>&, const Pieces&);

template<typename, typename Board> bool is_intersecting_capture(const Position<Board>&, const Pieces&);

// tag dispatching based on capture removal
template<typename Board> bool is_intersecting_capture(const Position<Board>&, const Pieces&, Int2Type<variants::REMOVE_1>);
template<typename Board> bool is_intersecting_capture(const Position<Board>&, const Pieces&, Int2Type<variants::REMOVE_N>);

template<typename> bool is_intersecting_capture(BitBoard, BitBoard);

// tag dispatching basd on capture removal
bool is_intersecting_capture(BitBoard, BitBoard, Int2Type<variants::REMOVE_1>);        
bool is_intersecting_capture(BitBoard, BitBoard, Int2Type<variants::REMOVE_N>);

template<typename> bool is_intersecting_promotion(BitBoard, BitBoard);

// tag dispatching based on promotion condition
bool is_intersecting_promotion(BitBoard, BitBoard, Int2Type<variants::PROMOTE_BR>);
bool is_intersecting_promotion(BitBoard, BitBoard, Int2Type<variants::PROMOTE_EP>);

}       // namespace node
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Predicates.hpp"
