#include "../Utilities/Bit.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

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
template<bool Color> FORCE_INLINE
void Move::push_back(List& v, BitBoard delta, BitBoard captured_pieces, BitBoard captured_kings)
{
        v.push();
        init<Color>(v.back(), delta, captured_pieces, captured_kings);
}

// add a man capture
template<bool Color> FORCE_INLINE
void Move::push_back(List& v, BitBoard delta, BitBoard promotion, BitBoard captured_pieces, BitBoard captured_kings)
{
        v.push();
        init<Color>(v.back(), delta, promotion, captured_pieces, captured_kings);
}

// king move
template<bool Color> FORCE_INLINE
void Move::init(Pieces& m, BitBoard delta)
{
        // necessary pre-conditions for move semantics
        assert(Bit::is_double(delta));

        m.init<Color>
        (
                delta,                          // move a king between the from and destination squares
                0,
                delta                           // move a king between the from and destination squares
        );
}

// man move
template<bool Color> FORCE_INLINE
void Move::init(Pieces& m, BitBoard delta, BitBoard promotion)
{
        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_within(promotion, delta));

        // necessary pre-conditions for move semantics
        assert(Bit::is_double(delta));
        assert(!Bit::is_multiple(promotion));

        m.init<Color>
        (
                delta,                          // move a man between the from and destination squares
                0,
                promotion                       // crown a king on the back row
        );
}

// king capture
template<bool Color> FORCE_INLINE
void Move::init(Pieces& m, BitBoard delta, BitBoard captured_pieces, BitBoard captured_kings)
{
        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_exclusive(delta, captured_pieces) || coup_thai(delta, captured_pieces));
        assert(Bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for move semantics
        assert(Bit::is_double(delta) || Bit::is_zero(delta));
        assert(!Bit::is_zero(captured_pieces));

        m.init<Color>
        (
                delta,                          // move a king between the from and destination square
                captured_pieces,                // remove the captured pieces
                delta ^ captured_kings          // move a king and remove the captured kings
        );
}

// man capture
template<bool Color> FORCE_INLINE
void Move::init(Pieces& m, BitBoard delta, BitBoard promotion, BitBoard captured_pieces, BitBoard captured_kings)
{
        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_within(promotion, delta) || promotion_en_passant(promotion, delta));
        assert(Bit::is_exclusive(delta, captured_pieces));
        assert(Bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for move semantics
        assert(Bit::is_double(delta) || Bit::is_zero(delta));
        assert(!Bit::is_multiple(promotion));
        assert(!Bit::is_zero(captured_pieces));

        m.init<Color>
        (
                delta,                          // move a man between the from and destination squares
                captured_pieces,                // remove the captured pieces
                promotion ^ captured_kings      // crown a king on the back row and remove the captured kings
        );
}
