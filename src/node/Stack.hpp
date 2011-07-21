#include <cassert>
#include "../utils/Bit.h"

namespace dtl {

// tag dispatching based on duplicate capture checking
template<typename Rules>
bool non_unique_top(const Stack& stack)
{
        return non_unique_top(stack, Int2Type<rules::is_check_capture_uniqueness<Rules>::value>());
}

// add a king move
template<bool Color>
void push(BitBoard delta, Stack& stack)
{
        // necessary pre-conditions for king move semantics
        assert(bit::is_double(delta));

        stack.increment_size();
        top(stack).init<Color>(
                delta,                          // move a king between the from and destination squares
                0,
                delta                           // move a king between the from and destination squares
        );
                
        // post-condtions are the pieces invariant 
        assert(top(stack).invariant());
}

// add a man move
template<bool Color>
void push(BitBoard delta, BitBoard promotion, Stack& stack)
{
        // necessary pre-conditions for the pieces invariant
        assert(bit::is_within(promotion, delta));

        // necessary pre-conditions for man move semantics
        assert(bit::is_double(delta));
        assert(!bit::is_multiple(promotion));

        stack.increment_size();
        top(stack).init<Color>(
                delta,                          // move a man between the from and destination squares
                0,
                promotion                       // crown a king on the back row
        );

        // post-conditions are the pieces invariant 
        assert(top(stack).invariant());
}

// add a king capture
template<bool Color, typename Rules>
void push(BitBoard delta, BitBoard captured_pieces, BitBoard captured_kings, Stack& stack)
{
        // necessary pre-conditions for the pieces invariant 
        assert(
                bit::is_exclusive(delta, captured_pieces) || 

                // EXCEPTION: for intersecting captures, delta overlaps with captured pieces
                is_intersecting_capture<Rules>(delta, captured_pieces)
        );
        assert(bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for king capture semantics
        assert(bit::is_double(delta) || bit::is_zero(delta));
        assert(!bit::is_zero(captured_pieces));

        stack.increment_size();
        top(stack).init<Color>(
                delta,                          // move a king between the from and destination square
                captured_pieces,                // remove the captured pieces
                delta ^ captured_kings          // move a king and remove the captured kings
        );

        // post-conditions are the pieces invariants                        
        assert(
                (
                        bit::is_exclusive(top(stack).pieces(Side::BLACK), top(stack).pieces(Side::WHITE)) ||

                        // EXCEPTION: for intersecting captures, WHITE and BLACK pieces() overlap
                        is_intersecting_capture<Rules>(delta, captured_pieces)
                ) &&
                bit::is_within(top(stack).kings(), top(stack).occupied())
        );
}

// add a man capture
template<bool Color, typename Rules>
void push(BitBoard delta, BitBoard promotion, BitBoard captured_pieces, BitBoard captured_kings, Stack& stack)
{
        // necessary pre-conditions for the pieces invariant
        assert(bit::is_exclusive(delta, captured_pieces));
        assert(
                bit::is_within(promotion, delta) ||

                // EXCEPTION: for intersecting promotions, delta is empty, and promotion is non-empty
                is_intersecting_promotion<Rules>(promotion, delta)
        );
        assert(bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for man capture semantics
        assert(bit::is_double(delta) || bit::is_zero(delta));
        assert(!bit::is_multiple(promotion));
        assert(!bit::is_zero(captured_pieces));

        stack.increment_size();
        top(stack).init<Color>(
                delta,                          // move a man between the from and destination squares
                captured_pieces,                // remove the captured pieces
                promotion ^ captured_kings      // crown a king and remove the captured kings
        );

        // post-conditions are the pieces invariants                        
        assert(
                bit::is_exclusive(top(stack).pieces(Side::BLACK), top(stack).pieces(Side::WHITE)) &&
                (
                        bit::is_within(top(stack).kings(), top(stack).occupied()) ||

                        // EXCEPTION: for intersecting promotions, kings() is non-empty, and occupied() is empty
                        is_intersecting_promotion<Rules>(promotion, delta)
                )
        );
}

}       // namespace dtl
