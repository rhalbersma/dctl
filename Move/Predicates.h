#pragma once
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/TemplateTricks.h"
#include "../Variants/Rules.h"

template<typename> class Position;
class Pieces;

namespace Move {

template<typename Board> bool is_connected(const Position<Board>&, const Pieces&, const Pieces&);
template<typename Board> bool is_non_conversion(const Position<Board>&, const Pieces&);
template<typename Board> bool is_promotion(const Position<Board>&, const Pieces&);
template<typename Board> bool is_with_king(const Position<Board>&, const Pieces&);
template<typename Board> bool is_capture(const Position<Board>&, const Pieces&);

template<typename Board> BitBoard moving_pieces(const Position<Board>&, const Pieces&);
template<typename Board> BitBoard moving_kings(const Position<Board>&, const Pieces&);
template<typename Board> BitBoard captured_pieces(const Position<Board>&, const Pieces&);
template<typename Board> BitBoard captured_kings(const Position<Board>&, const Pieces&);
template<typename Board> BitBoard from_sq(const Position<Board>&, const Pieces&);
template<typename Board> BitBoard dest_sq(const Position<Board>&, const Pieces&);

// pre-conditions for modifiers
template<typename, typename Board> bool is_pseudo_legal(const Position<Board>&, const Pieces&);

// tag dispatching on capture removal
template<typename, typename Board> bool is_sequential_capture_removal(const Position<Board>&, const Pieces&);
template<typename Board> bool is_sequential_capture_removal(const Position<Board>&, const Pieces&, Int2Type<Variants::REMOVE_1>);
template<typename Board> bool is_sequential_capture_removal(const Position<Board>&, const Pieces&, Int2Type<Variants::REMOVE_N>);

// tag dispatching on capture removal
template<typename> bool is_sequential_capture_removal(BitBoard, BitBoard);
bool is_sequential_capture_removal(BitBoard, BitBoard, Int2Type<Variants::REMOVE_1>);        
bool is_sequential_capture_removal(BitBoard, BitBoard, Int2Type<Variants::REMOVE_N>);

// tag dispatching based on promotion condition
template<typename> bool is_promotion_en_passant(BitBoard, BitBoard);
bool is_promotion_en_passant(BitBoard, BitBoard, Int2Type<Variants::PROMOTE_BR>);
bool is_promotion_en_passant(BitBoard, BitBoard, Int2Type<Variants::PROMOTE_EP>);

}       // namespace Move

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Predicates.hpp"
