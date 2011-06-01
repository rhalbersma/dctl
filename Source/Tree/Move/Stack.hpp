#include <cassert>
#include "../../Utilities/Bit.h"

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
        // necessary pre-conditions for king move semantics
        assert(Bit::is_double(delta));

        vector_array_.increment_size();
        top().init<Color>(
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
        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_within(promotion, delta));

        // necessary pre-conditions for man move semantics
        assert(Bit::is_double(delta));
        assert(!Bit::is_multiple(promotion));

        vector_array_.increment_size();
        top().init<Color>(
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
        // necessary pre-conditions for the pieces invariant 
        assert(
                Bit::is_exclusive(delta, captured_pieces) || 

                // EXCEPTION: for intersecting captures, delta overlaps with captured pieces
                is_intersecting_capture<Rules>(delta, captured_pieces)
        );
        assert(Bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for king capture semantics
        assert(Bit::is_double(delta) || Bit::is_zero(delta));
        assert(!Bit::is_zero(captured_pieces));

        vector_array_.increment_size();
        top().init<Color>(
                delta,                          // move a king between the from and destination square
                captured_pieces,                // remove the captured pieces
                delta ^ captured_kings          // move a king and remove the captured kings
        );

        // post-conditions are the pieces invariants                        
        assert(
                (
                        Bit::is_exclusive(top().pieces(Node::Side::BLACK), top().pieces(Node::Side::WHITE)) ||

                        // EXCEPTION: for intersecting captures, WHITE and BLACK pieces() overlap
                        is_intersecting_capture<Rules>(delta, captured_pieces)
                ) &&
                Bit::is_within(top().kings(), top().occupied())
        );
}

// add a man capture
template<bool Color, typename Rules>
void Stack::push(BitBoard delta, BitBoard promotion, BitBoard captured_pieces, BitBoard captured_kings)
{
        // necessary pre-conditions for the pieces invariant
        assert(Bit::is_exclusive(delta, captured_pieces));
        assert(
                Bit::is_within(promotion, delta) ||

                // EXCEPTION: for intersecting promotions, delta is empty, and promotion is non-empty
                is_intersecting_promotion<Rules>(promotion, delta)
        );
        assert(Bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for man capture semantics
        assert(Bit::is_double(delta) || Bit::is_zero(delta));
        assert(!Bit::is_multiple(promotion));
        assert(!Bit::is_zero(captured_pieces));

        vector_array_.increment_size();
        top().init<Color>(
                delta,                          // move a man between the from and destination squares
                captured_pieces,                // remove the captured pieces
                promotion ^ captured_kings      // crown a king and remove the captured kings
        );

        // post-conditions are the pieces invariants                        
        assert(
                Bit::is_exclusive(top().pieces(Node::Side::BLACK), top().pieces(Node::Side::WHITE)) &&
                (
                        Bit::is_within(top().kings(), top().occupied()) ||

                        // EXCEPTION: for intersecting promotions, kings() is non-empty, and occupied() is empty
                        is_intersecting_promotion<Rules>(promotion, delta)
                )
        );
}

}       // namespace Move
}       // namespace Tree
