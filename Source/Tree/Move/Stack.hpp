#include "../../Utilities/Bit.h"
#include <cassert>

namespace Tree {
namespace Move {

// tag dispatching based on duplicate capture checking
template<typename Rules>
bool Stack::non_unique_top(void)
{
        return non_unique_top(Int2Type<Variants::is_check_capture_uniqueness<Rules>::value>());
}

// add a king move
template<bool Color>
void Stack::push(BitBoard delta)
{
        vector_array_.increment_size();

        // necessary pre-conditions for king move semantics
        assert(Bit::is_double(delta));

        top().init<Color>
        (
                delta,                          // move a king between the from and destination squares
                0,
                delta                           // move a king between the from and destination squares
        );
                
        // post-condtions are the pieces invariant 
        assert(top().invariant());
}

// add a man move
template<bool Color>
void Stack::push(BitBoard delta, BitBoard promotion)
{
        vector_array_.increment_size();

        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_within(promotion, delta));

        // necessary pre-conditions for man move semantics
        assert(Bit::is_double(delta));
        assert(!Bit::is_multiple(promotion));

        top().init<Color>
        (
                delta,                          // move a man between the from and destination squares
                0,
                promotion                       // crown a king on the back row
        );

        // post-conditions are the pieces invariant 
        assert(top().invariant());
}

// add a king capture
template<bool Color, typename Rules>
void Stack::push(BitBoard delta, BitBoard captured_pieces, BitBoard captured_kings)
{
        vector_array_.increment_size();

        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_exclusive(delta, captured_pieces) || is_sequential_capture_removal<Rules>(delta, captured_pieces));
        assert(Bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for king capture semantics
        assert(Bit::is_double(delta) || Bit::is_zero(delta));
        assert(!Bit::is_zero(captured_pieces));

        top().init<Color>
        (
                delta,                          // move a king between the from and destination square
                captured_pieces,                // remove the captured pieces
                delta ^ captured_kings          // move a king and remove the captured kings
        );

        // post-conditions are the pieces invariants, with an exception for sequential capture removal (Thai draughts)
        // example: [FEN "W:WK26:B9,12,18,19"]; white has to capture 26x12, landing on a square it also captured on
        // (i.e. the moving and captured pieces are not mutually exclusive)
        assert(top().invariant() || is_sequential_capture_removal<Rules>(delta, captured_pieces));
}

// add a man capture
template<bool Color, typename Rules>
void Stack::push(BitBoard delta, BitBoard promotion, BitBoard captured_pieces, BitBoard captured_kings)
{
        vector_array_.increment_size();

        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_within(promotion, delta) || is_promotion_en_passant<Rules>(promotion, delta));
        assert(Bit::is_exclusive(delta, captured_pieces));
        assert(Bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for man capture semantics
        assert(Bit::is_double(delta) || Bit::is_zero(delta));
        assert(!Bit::is_multiple(promotion));
        assert(!Bit::is_zero(captured_pieces));

        top().init<Color>
        (
                delta,                          // move a man between the from and destination squares
                captured_pieces,                // remove the captured pieces
                promotion ^ captured_kings      // crown a king and remove the captured kings
        );

        // post-conditions are the pieces invariants, with an exception for promotion en-passant (Russian draughts)
        // example: [FEN "W:W25:B8,9,20,22,30"]; white has to capture 25x25, promoting on its original square
        // (i.e. the delta_kings is not contained within the delta_pieces)
        assert
        (
                Bit::is_exclusive(top().pieces(Node::Side::BLACK), top().pieces(Node::Side::WHITE)) &&
                (
                        Bit::is_within(top().kings(), top().occupied()) ||
                        is_promotion_en_passant<Rules>(promotion, delta)
                )
        );
}

}       // namespace Move
}       // namespace Tree
