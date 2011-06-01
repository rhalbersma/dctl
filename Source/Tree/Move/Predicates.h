#pragma once
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"
#include "../../Variants/Rules.h"

namespace Tree {

namespace Node { 
        template<typename> class Position;
        class Pieces;
}

namespace Move {

template<typename Board> bool is_connected(const Node::Position<Board>&, const Node::Pieces&, const Node::Pieces&);
template<typename Board> bool is_non_conversion(const Node::Position<Board>&, const Node::Pieces&);
template<typename Board> bool is_promotion(const Node::Position<Board>&, const Node::Pieces&);
template<typename Board> bool is_with_king(const Node::Position<Board>&, const Node::Pieces&);
template<typename Board> bool is_capture(const Node::Position<Board>&, const Node::Pieces&);

// pre-conditions for modifiers
template<typename, typename Board> bool is_pseudo_legal(const Node::Position<Board>&, const Node::Pieces&);

// tag dispatching on capture removal
template<typename, typename Board> bool is_intersecting_capture(const Node::Position<Board>&, const Node::Pieces&);
template<typename Board> bool is_intersecting_capture(const Node::Position<Board>&, const Node::Pieces&, Int2Type<Variants::REMOVE_1>);
template<typename Board> bool is_intersecting_capture(const Node::Position<Board>&, const Node::Pieces&, Int2Type<Variants::REMOVE_N>);

// tag dispatching on capture removal
template<typename> bool is_intersecting_capture(BitBoard, BitBoard);
bool is_intersecting_capture(BitBoard, BitBoard, Int2Type<Variants::REMOVE_1>);        
bool is_intersecting_capture(BitBoard, BitBoard, Int2Type<Variants::REMOVE_N>);

// tag dispatching based on promotion condition
template<typename> bool is_intersecting_promotion(BitBoard, BitBoard);
bool is_intersecting_promotion(BitBoard, BitBoard, Int2Type<Variants::PROMOTE_BR>);
bool is_intersecting_promotion(BitBoard, BitBoard, Int2Type<Variants::PROMOTE_EP>);

template<typename Board> BitBoard from_sq(const Node::Position<Board>&, const Node::Pieces&);
template<typename Board> BitBoard dest_sq(const Node::Position<Board>&, const Node::Pieces&);
template<typename Board> BitBoard moving_pieces(const Node::Position<Board>&, const Node::Pieces&);
template<typename Board> BitBoard moving_kings(const Node::Position<Board>&, const Node::Pieces&);
template<typename Board> BitBoard captured_pieces(const Node::Position<Board>&, const Node::Pieces&);
template<typename Board> BitBoard captured_kings(const Node::Position<Board>&, const Node::Pieces&);

}       // namespace Move
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Predicates.hpp"
