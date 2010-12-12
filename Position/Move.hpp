#include "../Utilities/Bit.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

// tag dispatching based on duplicate capture checking
template<typename Rules>
bool Move::non_unique_back(const List& v)
{
        return non_unique_back(v, Int2Type<is_CheckCaptureUniqueness<Rules>::VALUE>());
}

// add a king move
template<bool Color> FORCE_INLINE
void Move::push_back(List& v, BitBoard delta)
{
        v.push();
        init<Color>(v.back(), delta);
}

// add a man move
template<bool Color> FORCE_INLINE
void Move::push_back(List& v, BitBoard delta, BitBoard promotion)
{
        v.push();
        init<Color>(v.back(), delta, promotion);
}

// add a king capture
template<bool Color, typename Rules> FORCE_INLINE
void Move::push_back(List& v, BitBoard delta, BitBoard captured_pieces, BitBoard captured_kings)
{
        v.push();
        init<Color, Rules>(v.back(), delta, captured_pieces, captured_kings);
}

// add a man capture
template<bool Color, typename Rules> FORCE_INLINE
void Move::push_back(List& v, BitBoard delta, BitBoard promotion, BitBoard captured_pieces, BitBoard captured_kings)
{
        v.push();
        init<Color, Rules>(v.back(), delta, promotion, captured_pieces, captured_kings);
}

// king move
template<bool Color> FORCE_INLINE
void Move::init(Pieces& m, BitBoard delta)
{
        // necessary pre-conditions for king move semantics
        assert(Bit::is_double(delta));

        m.init<Color>
        (
                delta,                          // move a king between the from and destination squares
                0,
                delta                           // move a king between the from and destination squares
        );
                
        // post-condtions are the pieces invariant 
        assert(m.invariant());
}

// man move
template<bool Color> FORCE_INLINE
void Move::init(Pieces& m, BitBoard delta, BitBoard promotion)
{
        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_within(promotion, delta));

        // necessary pre-conditions for man move semantics
        assert(Bit::is_double(delta));
        assert(!Bit::is_multiple(promotion));

        m.init<Color>
        (
                delta,                          // move a man between the from and destination squares
                0,
                promotion                       // crown a king on the back row
        );

        // post-conditions are the pieces invariant 
        assert(m.invariant());
}

// king capture
template<bool Color, typename Rules> FORCE_INLINE
void Move::init(Pieces& m, BitBoard delta, BitBoard captured_pieces, BitBoard captured_kings)
{
        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_exclusive(delta, captured_pieces) || sequential_capture_removal<Rules>(delta, captured_pieces));
        assert(Bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for king capture semantics
        assert(Bit::is_double(delta) || Bit::is_zero(delta));
        assert(!Bit::is_zero(captured_pieces));

        m.init<Color>
        (
                delta,                          // move a king between the from and destination square
                captured_pieces,                // remove the captured pieces
                delta ^ captured_kings          // move a king and remove the captured kings
        );

        // post-conditions are the pieces invariants, with an exception for sequential capture removal (Thai draughts)
        // example: [FEN W:WK26:B9,12,18,19], white has to capture 26x12, landing on a square it also captured on
        assert(m.invariant() || sequential_capture_removal<Rules>(delta, captured_pieces));
}

// man capture
template<bool Color, typename Rules> FORCE_INLINE
void Move::init(Pieces& m, BitBoard delta, BitBoard promotion, BitBoard captured_pieces, BitBoard captured_kings)
{
        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_within(promotion, delta) || promotion_en_passant<Rules>(promotion, delta));
        assert(Bit::is_exclusive(delta, captured_pieces));
        assert(Bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for man capture semantics
        assert(Bit::is_double(delta) || Bit::is_zero(delta));
        assert(!Bit::is_multiple(promotion));
        assert(!Bit::is_zero(captured_pieces));

        m.init<Color>
        (
                delta,                          // move a man between the from and destination squares
                captured_pieces,                // remove the captured pieces
                promotion ^ captured_kings      // crown a king and remove the captured kings
        );

        // post-conditions are the pieces invariants, with an exception for promotion en-passant (Russian draughts)
        // example: [FEN W:W25:B8,9,20,22,30], white has to capture 25x25, promoting on its original square
        assert
        (
                Bit::is_exclusive(m.pieces(Side::BLACK), m.pieces(Side::WHITE)) &&
                (
                        Bit::is_within(m.kings(), m.occupied()) ||
                        promotion_en_passant<Rules>(promotion, delta)
                )
        );
}

// tag dispatching based on promotion condition
template<typename Rules>
bool Move::promotion_en_passant(BitBoard promotion, BitBoard delta)
{
        return promotion_en_passant(promotion, delta, Int2Type<PromotionCondition<Rules>::VALUE>());
}

// tag dispatching on capture removal
template<typename Rules>
bool Move::sequential_capture_removal(BitBoard delta, BitBoard captured_pieces)
{
        return sequential_capture_removal(delta, captured_pieces, Int2Type<CaptureRemoval<Rules>::VALUE>());
}
