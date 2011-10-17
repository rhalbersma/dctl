#pragma once
#include <algorithm>                    // std::find
#include <vector>                       // std::vector
#include <boost/assert.hpp>             // BOOST_ASSERT
#include "Move.hpp"
#include "Predicates.hpp"
#include "../bit/Bit.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

typedef std::vector<Move> Stack;

inline Stack::reference top(Stack& stack)
{
        return stack.back();
}

inline Stack::const_reference top(const Stack& stack)
{
        return stack.back();
}

template<typename Rules>
bool non_unique_top(const Stack& stack)
{
        // tag dispatching on duplicate capture checking
        return non_unique_top_dispatch(
                stack, 
                Int2Type<rules::is_check_capture_uniqueness<Rules>::value>()
        );
}

// specialization for move generation without duplicate capture checking
inline bool non_unique_top_dispatch(const Stack&, Int2Type<false>)
{
        return false;
}

// specialization for move generation without duplicate capture checking
inline bool non_unique_top_dispatch(const Stack& stack, Int2Type<true>)
{
        return std::find(stack.begin(), stack.end(), top(stack)) != (stack.end() - 1);
}

// add a king move
template<bool Color>
void push(BitBoard delta, Stack& stack)
{
        // necessary pre-conditions for king move semantics
        BOOST_ASSERT(bit::is_double(delta));

        stack.push_back(
                Move::create<Color>(
                        delta,                          // move a king between the from and destination squares
                        0,
                        delta                           // move a king between the from and destination squares
                )
        );
                
        // post-condtions are the pieces invariant 
        BOOST_ASSERT(top(stack).invariant());
}

// add a man move
template<bool Color>
void push(BitBoard delta, BitBoard promotion, Stack& stack)
{
        // necessary pre-conditions for the pieces invariant
        BOOST_ASSERT(bit::is_within(promotion, delta));

        // necessary pre-conditions for man move semantics
        BOOST_ASSERT(bit::is_double(delta));
        BOOST_ASSERT(!bit::is_multiple(promotion));

        stack.push_back(
                Move::create<Color>(
                        delta,                          // move a man between the from and destination squares
                        0,
                        promotion                       // crown a king on the back row
                )
        );

        // post-conditions are the pieces invariant 
        BOOST_ASSERT(top(stack).invariant());
}

// add a king capture
template<bool Color, typename Rules>
void push(BitBoard delta, BitBoard captured_pieces, BitBoard captured_kings, Stack& stack)
{
        // necessary pre-conditions for the pieces invariant 
        BOOST_ASSERT(
                bit::is_exclusive(delta, captured_pieces) || 

                // EXCEPTION: for intersecting captures, delta overlaps with captured pieces
                is_intersecting_capture<Rules>(delta, captured_pieces)
        );
        BOOST_ASSERT(bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for king capture semantics
        BOOST_ASSERT(bit::is_double(delta) || bit::is_zero(delta));
        BOOST_ASSERT(!bit::is_zero(captured_pieces));

        stack.push_back(
                Move::create<Color>(
                        delta,                          // move a king between the from and destination square
                        captured_pieces,                // remove the captured pieces
                        delta ^ captured_kings          // move a king and remove the captured kings
                )
        );

        // post-conditions are the pieces invariants                        
        BOOST_ASSERT(
                (
                        bit::is_exclusive(top(stack).pieces(Side::black), top(stack).pieces(Side::white)) ||

                        // EXCEPTION: for intersecting captures, WHITE and BLACK pieces() overlap
                        is_intersecting_capture<Rules>(delta, captured_pieces)
                ) &&
                bit::is_within(top(stack).kings(), top(stack).pieces())
        );
}

// add a man capture
template<bool Color, typename Rules>
void push(BitBoard delta, BitBoard promotion, BitBoard captured_pieces, BitBoard captured_kings, Stack& stack)
{
        // necessary pre-conditions for the pieces invariant
        BOOST_ASSERT(bit::is_exclusive(delta, captured_pieces));
        BOOST_ASSERT(
                bit::is_within(promotion, delta) ||

                // EXCEPTION: for intersecting promotions, delta is empty, and promotion is non-empty
                is_intersecting_promotion<Rules>(promotion, delta)
        );
        BOOST_ASSERT(bit::is_within(captured_kings, captured_pieces));

        // necessary pre-conditions for man capture semantics
        BOOST_ASSERT(bit::is_double(delta) || bit::is_zero(delta));
        BOOST_ASSERT(!bit::is_multiple(promotion));
        BOOST_ASSERT(!bit::is_zero(captured_pieces));

        stack.push_back(
                Move::create<Color>(
                        delta,                          // move a man between the from and destination squares
                        captured_pieces,                // remove the captured pieces
                        promotion ^ captured_kings      // crown a king and remove the captured kings
                )
        );

        // post-conditions are the pieces invariants                        
        BOOST_ASSERT(
                bit::is_exclusive(top(stack).pieces(Side::black), top(stack).pieces(Side::white)) &&
                (
                        bit::is_within(top(stack).kings(), top(stack).pieces()) ||

                        // EXCEPTION: for intersecting promotions, kings() is non-empty, and occupied() is empty
                        is_intersecting_promotion<Rules>(promotion, delta)
                )
        );
}

inline void pop(Stack& stack)
{
        return stack.pop_back();
}

}       // namespace dctl
