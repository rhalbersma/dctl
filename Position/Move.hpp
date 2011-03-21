#include "../Utilities/Bit.h"
#include <cassert>

namespace Move {

// tag dispatching based on duplicate capture checking
template<typename Rules>
bool Move::List::non_unique_back(void)
{
        return non_unique_back(Int2Type<Variants::is_CheckCaptureUniqueness<Rules>::VALUE>());
}

// add a king move
template<bool Color>
void Move::List::push_back(BitBoard delta)
{
        move_list_.push();

        // necessary pre-conditions for king move semantics
        assert(Bit::is_double(delta));

        move_list_.back().init<Color>
        (
                delta,                          // move a king between the from and destination squares
                0,
                delta                           // move a king between the from and destination squares
        );
                
        // post-condtions are the pieces invariant 
        assert(move_list_.back().invariant());
}

// add a man move
template<bool Color>
void Move::List::push_back(BitBoard delta, BitBoard promotion)
{
        move_list_.push();

        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_within(promotion, delta));

        // necessary pre-conditions for man move semantics
        assert(Bit::is_double(delta));
        assert(!Bit::is_multiple(promotion));

        move_list_.back().init<Color>
        (
                delta,                          // move a man between the from and destination squares
                0,
                promotion                       // crown a king on the back row
        );

        // post-conditions are the pieces invariant 
        assert(move_list_.back().invariant());
}

// add a king capture
template<bool Color, typename Rules>
void Move::List::push_back(BitBoard delta, BitBoard captured_pieces, BitBoard captured_kings)
{
        move_list_.push();

        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_exclusive(delta, captured_pieces) || sequential_capture_removal<Rules>(delta, captured_pieces));
        assert(Bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for king capture semantics
        assert(Bit::is_double(delta) || Bit::is_zero(delta));
        assert(!Bit::is_zero(captured_pieces));

        move_list_.back().init<Color>
        (
                delta,                          // move a king between the from and destination square
                captured_pieces,                // remove the captured pieces
                delta ^ captured_kings          // move a king and remove the captured kings
        );

        // post-conditions are the pieces invariants, with an exception for sequential capture removal (Thai draughts)
        // example: [FEN "W:WK26:B9,12,18,19"]; white has to capture 26x12, landing on a square it also captured on
        assert(move_list_.back().invariant() || sequential_capture_removal<Rules>(delta, captured_pieces));
}

// add a man capture
template<bool Color, typename Rules>
void Move::List::push_back(BitBoard delta, BitBoard promotion, BitBoard captured_pieces, BitBoard captured_kings)
{
        move_list_.push();

        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_within(promotion, delta) || promotion_en_passant<Rules>(promotion, delta));
        assert(Bit::is_exclusive(delta, captured_pieces));
        assert(Bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for man capture semantics
        assert(Bit::is_double(delta) || Bit::is_zero(delta));
        assert(!Bit::is_multiple(promotion));
        assert(!Bit::is_zero(captured_pieces));

        move_list_.back().init<Color>
        (
                delta,                          // move a man between the from and destination squares
                captured_pieces,                // remove the captured pieces
                promotion ^ captured_kings      // crown a king and remove the captured kings
        );

        // post-conditions are the pieces invariants, with an exception for promotion en-passant (Russian draughts)
        // example: [FEN "W:W25:B8,9,20,22,30"]; white has to capture 25x25, promoting on its original square
        assert
        (
                Bit::is_exclusive(move_list_.back().pieces(Side::BLACK), move_list_.back().pieces(Side::WHITE)) &&
                (
                        Bit::is_within(move_list_.back().kings(), move_list_.back().occupied()) ||
                        promotion_en_passant<Rules>(promotion, delta)
                )
        );
}

// tag dispatching based on promotion condition
template<typename Rules>
bool Move::List::promotion_en_passant(BitBoard promotion, BitBoard delta)
{
        return promotion_en_passant(promotion, delta, Int2Type<Variants::PromotionCondition<Rules>::VALUE>());
}

// tag dispatching on capture removal
template<typename Rules>
bool Move::List::sequential_capture_removal(BitBoard delta, BitBoard captured_pieces)
{
        return sequential_capture_removal(delta, captured_pieces, Int2Type<Variants::CaptureRemoval<Rules>::VALUE>());
}

}       // namespace Move
