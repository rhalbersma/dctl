#pragma once
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/TemplateTricks.h"
#include "../../Variants/Rules.h"

namespace tree {

namespace node { 
        template<typename> class Position;
        class Pieces;
}

namespace move {

template<typename Board> bool is_connected(const node::Position<Board>&, const node::Pieces&, const node::Pieces&);
template<typename Board> bool is_non_conversion(const node::Position<Board>&, const node::Pieces&);
template<typename Board> bool is_promotion(const node::Position<Board>&, const node::Pieces&);
template<typename Board> bool is_with_king(const node::Position<Board>&, const node::Pieces&);
template<typename Board> bool is_capture(const node::Position<Board>&, const node::Pieces&);

// pre-conditions for modifiers
template<typename, typename Board> bool is_pseudo_legal(const node::Position<Board>&, const node::Pieces&);

// tag dispatching on capture removal
template<typename, typename Board> bool is_intersecting_capture(const node::Position<Board>&, const node::Pieces&);
template<typename Board> bool is_intersecting_capture(const node::Position<Board>&, const node::Pieces&, Int2Type<variants::REMOVE_1>);
template<typename Board> bool is_intersecting_capture(const node::Position<Board>&, const node::Pieces&, Int2Type<variants::REMOVE_N>);

// tag dispatching on capture removal
template<typename> bool is_intersecting_capture(BitBoard, BitBoard);
bool is_intersecting_capture(BitBoard, BitBoard, Int2Type<variants::REMOVE_1>);        
bool is_intersecting_capture(BitBoard, BitBoard, Int2Type<variants::REMOVE_N>);

// tag dispatching based on promotion condition
template<typename> bool is_intersecting_promotion(BitBoard, BitBoard);
bool is_intersecting_promotion(BitBoard, BitBoard, Int2Type<variants::PROMOTE_BR>);
bool is_intersecting_promotion(BitBoard, BitBoard, Int2Type<variants::PROMOTE_EP>);

template<typename Board> BitBoard from_sq(const node::Position<Board>&, const node::Pieces&);
template<typename Board> BitBoard dest_sq(const node::Position<Board>&, const node::Pieces&);
template<typename Board> BitBoard moving_pieces(const node::Position<Board>&, const node::Pieces&);
template<typename Board> BitBoard moving_kings(const node::Position<Board>&, const node::Pieces&);
template<typename Board> BitBoard captured_pieces(const node::Position<Board>&, const node::Pieces&);
template<typename Board> BitBoard captured_kings(const node::Position<Board>&, const node::Pieces&);

}       // namespace move
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Predicates.hpp"
