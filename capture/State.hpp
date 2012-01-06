#pragma once
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/config.hpp>                     // BOOST_STATIC_CONSTANT
#include "Value.hpp"
#include "../bit/Bit.hpp"
#include "../node/Promotion.hpp"
#include "../node/Position.hpp"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/Shift.hpp"

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
                BOOST_ASSERT(invariant());
        }

        bool invariant() const
        {
                return (
                        !is_multiple(from_sq_) && 
                        bit::is_within(remaining_targets_, initial_targets_)
                );
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
        bool is_promotion_sq(BitBoard dest_sq) const
        {
                return !bit::is_zero(promotion_sq<Color, Board>(dest_sq));
        }
        
        // modifiers before a capture
        void toggle_king_targets()
        {
                initial_targets_ = remaining_targets_ ^= king_targets_;
        }
        
        void current_toggle_with_king()
        {
                toggle_with_king(current_);
        }

        void current_toggle_promotion()
        {
                toggle_promotion(current_);
        }

        // modifiers during a capture
        void launch(BitBoard jump_sq)
        {
                from_sq_ = jump_sq;
                not_occupied_ ^= jump_sq;
                BOOST_ASSERT(invariant());
        }

        void finish(BitBoard jump_sq)
        {
                not_occupied_ ^= jump_sq;
                from_sq_ = BitBoard(0);
                BOOST_ASSERT(invariant());
        }

        void make(BitBoard target_sq)
        {
                // tag dispatching on capture removal
                make_dispatch(
                        target_sq, 
                        Int2Type<rules::capture_removal<Rules>::value>()
                );                                
                BOOST_ASSERT(invariant());
        }

        void undo(BitBoard target_sq)
        {
                // tag dispatching on capture removal
                undo_dispatch(
                        target_sq, 
                        Int2Type<rules::capture_removal<Rules>::value>()
                );
                BOOST_ASSERT(invariant());
        }

        template<bool Color> 
        void add_man_capture(BitBoard dest_sq, Stack& moves) const
        {
                // tag dispatching on ambiguity of man captures
                add_man_capture_dispatch<Color>(
                        dest_sq, moves, 
                        Int2Type<rules::is_ambiguous_man_capture<Rules>::value>()
                );
        }
        
        template<bool Color, int Index> 
        void add_king_capture(BitBoard dest_sq, Stack& moves) const
        {
                const auto ambiguous = !moves.empty() && current_is_large();

                // tag dispatching on king halt after final capture
                add_king_capture_dispatch<Color, Index>(
                        dest_sq, ambiguous, moves, 
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

        BitBoard captured_king_targets() const
        {
                // tag dispatching on whether men can capture kings
                return captured_king_targets_dispatch(
                        Int2Type<rules::is_men_capture_kings<Rules>::value>()
                );
        }

        // specialization for men that cannot capture kings
        BitBoard captured_king_targets_dispatch(
                Int2Type<false>
        ) const
        {
                return BitBoard(0);
        }

        // specialization for men that can capture kings
        BitBoard captured_king_targets_dispatch(
                Int2Type<true>
        ) const
        {
                return captured_kings();
        }

        // specialization for apres-fini capture removal
        void make_dispatch(
                BitBoard target_sq, Int2Type<rules::remove_af>
        )
        {
                remaining_targets_ ^= target_sq;
                current_increment(target_sq);
        }

        // specialization for en-passant capture removal
        void make_dispatch(
                BitBoard target_sq, Int2Type<rules::remove_ep>
        )
        {
                not_occupied_ ^= target_sq;
                make_dispatch(target_sq, Int2Type<rules::remove_af>());
        }

        // specialization for apres-fini capture removal
        void undo_dispatch(
                BitBoard target_sq, Int2Type<rules::remove_af>
        )
        {
                current_decrement(target_sq);
                remaining_targets_ ^= target_sq;
        }

        // specialization for en-passant capture removal
        void undo_dispatch(
                BitBoard target_sq, Int2Type<rules::remove_ep>
        )
        {
                undo_dispatch(target_sq, Int2Type<rules::remove_af>());
                not_occupied_ ^= target_sq;
        }

        void current_increment(BitBoard target_sq)
        {
                current_increment_dispatch(
                        target_sq,
                        Int2Type<rules::is_qualified_majority<Rules>::value>()
                );
        }

        void current_increment_dispatch(
                BitBoard /* target_sq */, Int2Type<false>
        )
        {
                increment(current_);
        }

        void current_increment_dispatch(
                BitBoard target_sq, Int2Type<true>
        )
        {
                increment(current_, target_sq, king_targets_);
        }

        void current_decrement(BitBoard target_sq)
        {
                current_decrement_dispatch(
                        target_sq,
                        Int2Type<rules::is_qualified_majority<Rules>::value>()
                );
        }

        void current_decrement_dispatch(
                BitBoard /* target_sq */, Int2Type<false>
        )
        {
                decrement(current_);
        }

        void current_decrement_dispatch(
                BitBoard target_sq, Int2Type<true>
        )
        {
                decrement(current_, target_sq, king_targets_);
        }

        // partial specialization for man captures that are unambiguous
        template<bool Color> 
        void add_man_capture_dispatch(
                BitBoard dest_sq, Stack& moves, Int2Type<false>
        ) const
        {
                push<Color, Rules>(
                        from_sq_ ^ dest_sq, 
                        promotion_sq<Color, Board>(dest_sq), 
                        captured_pieces(), 
                        captured_king_targets(), 
                        moves
                );
        }

        // partial specialization for man captures that can be ambiguous
        template<bool Color> 
        void add_man_capture_dispatch(
                BitBoard dest_sq, Stack& moves, Int2Type<true>
        ) const
        {
                const auto ambiguous = !moves.empty() && current_is_large();
                add_man_capture_dispatch<Color>(dest_sq, moves, Int2Type<false>());
                if (ambiguous && non_unique_top<Rules>(moves))
                        pop(moves);
        }

        // partial specialization for kings that halt immediately if the final capture is a king, 
        // and slide through otherwise
        template<bool Color, int Index> 
        void add_king_capture_dispatch(
                BitBoard dest_sq, bool ambiguous, Stack& moves, Int2Type<rules::halt_1K>
        ) const
        {
                if (king_targets_ & Pull<Board, Index>()(dest_sq))
                        add_king_capture_dispatch<Color, Index>(dest_sq, ambiguous, moves, Int2Type<rules::halt_1>());
                else
                        add_king_capture_dispatch<Color, Index>(dest_sq, ambiguous, moves, Int2Type<rules::halt_N>());
        }
        
        // partial specialization for kings that halt immediately after the final capture
        template<bool Color, int Index> 
        void add_king_capture_dispatch(
                BitBoard dest_sq, bool ambiguous, Stack& moves, Int2Type<rules::halt_1>
        ) const
        {
                add_king_capture<Color>(dest_sq, ambiguous, moves);
        }
        
        // partial specialization for kings that slide through after the final capture
        template<bool Color, int Index> 
        void add_king_capture_dispatch(
                BitBoard dest_sq, bool ambiguous, Stack& moves, Int2Type<rules::halt_N>
        ) const
        {
                BOOST_ASSERT(dest_sq & path());
                do {
                        add_king_capture<Color>(dest_sq, ambiguous, moves);
                        PushAssign<Board, Index>()(dest_sq);
                } while (dest_sq & path());
        }

        template<bool Color> 
        void add_king_capture(BitBoard dest_sq, bool ambiguous, Stack& moves) const
        {
                // tag dispatching on promotion condition
                add_king_capture_dispatch<Color>(
                        dest_sq, moves, 
                        Int2Type<rules::promotion_condition<Rules>::value>()
                );
                if (ambiguous && non_unique_top<Rules>(moves))
                        pop(moves);
        }
        
        // partial specialization for men that promote apres-fini
        template<bool Color> 
        void add_king_capture_dispatch(
                BitBoard dest_sq, Stack& moves, 
                Int2Type<rules::promote_af>
        ) const
        {
                push<Color, Rules>(
                        from_sq_ ^ dest_sq, 
                        captured_pieces(), 
                        captured_kings(), 
                        moves
                );
        }

        // partial specialization for men that promote en-passant
        template<bool Color> 
        void add_king_capture_dispatch(
                BitBoard dest_sq, Stack& moves, Int2Type<rules::promote_ep>
        ) const
        {
                if (!is_promotion(current_))
                        add_king_capture_dispatch<Color>(dest_sq, moves, Int2Type<rules::promote_af>());
                else
                        push<Color, Rules>(
                                from_sq_ ^ dest_sq, 
                                dest_sq, 
                                captured_pieces(), 
                                captured_kings(), 
                                moves
                        );
        }

        bool current_is_large() const
        {
                return current_count() >= rules::large_capture<Rules>::value;
        }

        int current_count() const
        {
                return current_count_dispatch(
                        Int2Type<rules::is_majority_precedence<Rules>::value>()        
                );
        }

        int current_count_dispatch(
                Int2Type<false>
        ) const
        {
                return bit::count(captured_pieces());
        }

        int current_count_dispatch(
                Int2Type<true>
        ) const
        {
                return current_.count();
        }

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
