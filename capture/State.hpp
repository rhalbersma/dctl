#pragma once
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/config.hpp>                     // BOOST_STATIC_CONST
#include "Value.hpp"
#include "../bit/Bit.hpp"
#include "../node/Promotion.hpp"
#include "../node/Position.h"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/Shift.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {       
namespace capture {

template<typename Rules, typename Board>
class State
{
public:
        // constructors
        explicit State(const Position<Board>& p)
        :
                initial_targets_(passive_pieces(p)),
	        remaining_targets_(initial_targets_),
	        not_occupied_(not_occupied(p)),
                king_targets_(passive_kings(p)),
                from_sq_(0)
        {
        }

        // views
        template<int Index> 
        BitBoard targets() const
        {
                return remaining_targets_ & Pull<Board, Index>()(path());
        }
        
        template<int Index> 
        BitBoard path() const
        {
                return path() & Board::jump_start[Index];
        }
        
        BitBoard path() const
        {
                return not_occupied_;
        }

        const Value<Rules>& current() const
        {
                return current_;
        }
        
        const Value<Rules>& best() const
        {
                return best_;
        }

        Value<Rules>& best()
        {
                return best_;
        }

        // unary predicates
        template<bool Color> 
        bool is_promotion(BitBoard dest_sq) const
        {
                return !bit::is_zero(promotion_sq<Color, Board>(dest_sq));
        }
        
        bool is_promotion() const
        {
                return current_.is_promotion();
        }

        // modifiers before a capture
        void toggle_king_targets()
        {
                initial_targets_ = remaining_targets_ ^= king_targets_;
        }
        
        void toggle_with_king()
        {
                current_.toggle_with_king();
        }

        void toggle_promotion()
        {
                current_.toggle_promotion();
        }

        // modifiers during a capture
        void launch(BitBoard jump_sq)
        {
                from_sq_ = jump_sq;
                not_occupied_ ^= jump_sq;
        }

        void finish(BitBoard jump_sq)
        {
                not_occupied_ ^= jump_sq;
                from_sq_ = BitBoard(0);
        }

        // tag dispatching on capture removal
        void make(BitBoard target_sq)
        {
                make(
                        target_sq, 
                        Int2Type<rules::capture_removal<Rules>::value>()
                );
        }

        // tag dispatching on capture removal
        void undo(BitBoard target_sq)
        {
                undo(
                        target_sq, 
                        Int2Type<rules::capture_removal<Rules>::value>()
                );
        }

        // tag dispatching on ambiguity of man captures
        template<bool Color> 
        void add_man_capture(BitBoard dest_sq, Stack& move_stack) const
        {
                add_man_capture<Color>(
                        dest_sq, move_stack, 
                        Int2Type<rules::is_ambiguous_man_capture<Rules>::value>()
                );
        }
        
        // tag dispatching on king halt after final capture
        template<bool Color, int Index> 
        void add_king_capture(BitBoard dest_sq, Stack& move_stack) const
        {
                add_king_capture<Color, Index>(
                        dest_sq, move_stack, 
                        Int2Type<rules::king_capture_halt<Rules>::value>()
                );
        }

private:
        BitBoard captured_pieces() const
        {
                return initial_targets_ ^ remaining_targets_;
        }

        BitBoard captured_kings() const
        {
                return captured_pieces() & king_targets_;
        }

        // tag dispatching on whether men can capture kings
        BitBoard captured_king_targets() const
        {
                return captured_king_targets(
                        Int2Type<rules::is_men_capture_kings<Rules>::value>()
                );
        }

        // specialization for men that cannot capture kings
        BitBoard captured_king_targets(Int2Type<false>) const
        {
                return BitBoard(0);
        }

        // specialization for men that can capture kings
        BitBoard captured_king_targets(Int2Type<true>) const
        {
                return captured_kings();
        }

        // specialization for en-passant capture removal
        void make(BitBoard target_sq, Int2Type<rules::remove_ep>)
        {
                not_occupied_ ^= target_sq;
                make(target_sq, Int2Type<rules::remove_af>());
        }

        // specialization for apres-fini capture removal
        void make(BitBoard target_sq, Int2Type<rules::remove_af>)
        {
                remaining_targets_ ^= target_sq;
                current_.increment(target_sq, king_targets_);
        }

        // specialization for en-passant capture removal
        void undo(BitBoard target_sq, Int2Type<rules::remove_ep>)
        {
                undo(target_sq, Int2Type<rules::remove_af>());
                not_occupied_ ^= target_sq;
        }

        // specialization for apres-fini capture removal
        void undo(BitBoard target_sq, Int2Type<rules::remove_af>)
        {
                current_.decrement(target_sq, king_targets_);
                remaining_targets_ ^= target_sq;
        }

        // partial specialization for man captures that are unambiguous
        template<bool Color> 
        void add_man_capture(BitBoard dest_sq, Stack& move_stack, Int2Type<false>) const
        {
                push<Color, Rules>(
                        from_sq_ ^ dest_sq, 
                        promotion_sq<Color, Board>(dest_sq), 
                        captured_pieces(), 
                        captured_king_targets(), 
                        move_stack
                );
        }

        // partial specialization for man captures that can be ambiguous
        template<bool Color> 
        void add_man_capture(BitBoard dest_sq, Stack& move_stack, Int2Type<true>) const
        {
                const bool ambiguous = !move_stack.empty() && current_.is_large(captured_pieces());               
                add_man_capture<Color>(dest_sq, move_stack, Int2Type<false>());              
                if (ambiguous && non_unique_top<Rules>(move_stack))
                        pop(move_stack);
        }

        // partial specialization for kings that halt immediately if the final capture is a king, 
        // and slide through otherwise
        template<bool Color, int Index> 
        void add_king_capture(BitBoard dest_sq, Stack& move_stack, Int2Type<rules::halt_1K>) const
        {
                if (king_targets_ & Pull<Board, Index>()(dest_sq))
                        add_king_capture<Color, Index>(dest_sq, move_stack, Int2Type<rules::halt_1>());
                else
                        add_king_capture<Color, Index>(dest_sq, move_stack, Int2Type<rules::halt_N>());
        }
        
        // partial specialization for kings that halt immediately after the final capture
        template<bool Color, int Index> 
        void add_king_capture(BitBoard dest_sq, Stack& move_stack, Int2Type<rules::halt_1>) const
        {
                const bool ambiguous = !move_stack.empty() && current_.is_large(captured_pieces());
                add_king_capture<Color>(dest_sq, ambiguous, move_stack);
        }
        
        // partial specialization for kings that slide through after the final capture
        template<bool Color, int Index> 
        void add_king_capture(BitBoard dest_sq, Stack& move_stack, Int2Type<rules::halt_N>) const
        {
                BOOST_ASSERT(dest_sq & path());
                const bool ambiguous = !move_stack.empty() && current_.is_large(captured_pieces());
                do {
                        add_king_capture<Color>(dest_sq, ambiguous, move_stack);
                        PushAssign<Board, Index>()(dest_sq);
                } while (dest_sq & path());
        }

        // tag dispatching on promotion condition
        template<bool Color> 
        void add_king_capture(BitBoard dest_sq, bool ambiguous, Stack& move_stack) const
        {
                add_king_capture<Color>(
                        dest_sq, move_stack, 
                        Int2Type<rules::promotion_condition<Rules>::value>()
                );
                if (ambiguous && non_unique_top<Rules>(move_stack))
                        pop(move_stack);
        }
        
        // partial specialization for men that promote en-passant
        template<bool Color> 
        void add_king_capture(BitBoard dest_sq, Stack& move_stack, Int2Type<rules::promote_ep>) const
        {
                if (!is_promotion())
                        add_king_capture<Color>(dest_sq, move_stack, Int2Type<rules::promote_af>());
                else
                        push<Color, Rules>(
                                from_sq_ ^ dest_sq, 
                                dest_sq, 
                                captured_pieces(), 
                                captured_kings(), 
                                move_stack
                        );
        }
        
        // partial specialization for men that promote apres-fini
        template<bool Color> 
        void add_king_capture(BitBoard dest_sq, Stack& move_stack, Int2Type<rules::promote_af>) const
        {
                push<Color, Rules>(
                        from_sq_ ^ dest_sq, 
                        captured_pieces(), 
                        captured_kings(), 
                        move_stack
                );
        }

        // implementation
	BOOST_STATIC_CONSTANT(auto, TOGGLE = true);

        // representation
        BitBoard initial_targets_;      // targets before a capture
        BitBoard remaining_targets_;    // targets after a capture
        BitBoard not_occupied_;
        BitBoard king_targets_;
        BitBoard from_sq_;                                             
        Value<Rules> current_;
        Value<Rules> best_;
};

}       // namespace capture
}       // namespace dctl
