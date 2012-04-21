#pragma once
#include <algorithm>                    // find
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT
#include <boost/utility.hpp>            // noncopyable
#include "Value.hpp"
#include "../bit/Bit.hpp"
#include "../board/Shift.hpp"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace capture {

template<typename Rules, typename Board>
class State
:
        // enforce reference semantics
        private boost::noncopyable
{
public:
        // structors

        template<template<typename, typename> class Position>
        explicit State(const Position<Rules, Board>& p, Stack& m)
        :
                king_targets_(passive_kings(p)),
                initial_targets_(passive_pieces(p)),
                remaining_targets_(initial_targets_),
                not_occupied_(not_occupied(p)),
                from_sq_(0),
                current_(),
                best_(),
                moves_(m)
        {
                BOOST_ASSERT(invariant());
        }

        // modifiers 

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
                        Int2Type<rules::jump_removal<Rules>::value>()
                );
                BOOST_ASSERT(invariant());
        }

        void undo(BitBoard target_sq)
        {
                // tag dispatching on capture removal
                undo_dispatch(
                        target_sq,
                        Int2Type<rules::jump_removal<Rules>::value>()
                );
                BOOST_ASSERT(invariant());
        }

        void toggle_with_king()
        {              
                BOOST_STATIC_ASSERT(rules::is_relative_king_precedence<Rules>::value);
                current_.toggle_with_king();
        }

        void toggle_king_targets()
        {
                BOOST_STATIC_ASSERT(!rules::is_pawns_jump_kings<Rules>::value);
                initial_targets_ = remaining_targets_ ^= king_targets_;
        }

        void toggle_promotion()
        {
                BOOST_STATIC_ASSERT(rules::promotion_condition<Rules>::value == rules::promote_ep);
                current_.toggle_promotion();
        }

        void improve()
        {
                BOOST_ASSERT(rules::is_majority_precedence<Rules>::value);
                best_ = current_;
                moves_.clear();
        }

        template<bool Color>
        void add_pawn_jump(BitBoard dest_sq) const // modifies Stack& moves_
        {
                // tag dispatching on ambiguity of man captures
                add_pawn_jump_dispatch<Color>(
                        dest_sq,
                        Int2Type<rules::is_ambiguous_pawn_jump<Rules>::value>()
                );
        }

        template<bool Color, int Index>
        void add_king_jump(BitBoard dest_sq) const // modifies Stack& moves_
        {
                const auto ambiguous = is_ambiguous();

                // tag dispatching on king halt after final capture
                add_king_jump_dispatch<Color, Index>(
                        dest_sq, ambiguous,
                        Int2Type<rules::king_jump_halt<Rules>::value>()
                );
        }

        // queries

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

        bool is_improvement() const
        {
                BOOST_ASSERT(is_totally_ordered(best_, current_));
                return best_ <= current_;
        }

        bool improvement_is_strict() const
        {
                BOOST_ASSERT(is_improvement());
                return best_ != current_;
        }

        bool empty() const
        {
                return moves_.empty();
        }

private:
        // modifiers

        // specialization for apres-fini capture removal
        void make_dispatch(
                BitBoard target_sq, 
                Int2Type<rules::remove_af>
        )
        {
                remaining_targets_ ^= target_sq;
                increment(is_captured_king(target_sq));
        }

        // specialization for en-passant capture removal
        void make_dispatch(
                BitBoard target_sq, 
                Int2Type<rules::remove_ep>
        )
        {
                BOOST_STATIC_ASSERT(rules::jump_removal<Rules>::value == rules::remove_ep);
                not_occupied_ ^= target_sq;
                make_dispatch(
                        target_sq, 
                        Int2Type<rules::remove_af>()
                );
        }

        // specialization for apres-fini capture removal
        void undo_dispatch(
                BitBoard target_sq, 
                Int2Type<rules::remove_af>
        )
        {
                decrement(is_captured_king(target_sq));
                remaining_targets_ ^= target_sq;
        }

        // specialization for en-passant capture removal
        void undo_dispatch(
                BitBoard target_sq, 
                Int2Type<rules::remove_ep>
        )
        {
                BOOST_STATIC_ASSERT(rules::jump_removal<Rules>::value == rules::remove_ep);
                undo_dispatch(
                        target_sq, 
                        Int2Type<rules::remove_af>()
                );
                not_occupied_ ^= target_sq;
        }

        void increment(bool is_captured_king)
        {
                // tag dispatching on the type of capture precedence
                increment_dispatch(
                        is_captured_king,
                        Int2Type<rules::precedence_type<Rules>::value>()
                );
        }

        // specialization for no capture precedence
        void increment_dispatch(
                bool /* is_captured_king */, 
                Int2Type<rules::precede_0>
        )
        {
                BOOST_STATIC_ASSERT(rules::precedence_type<Rules>::value == rules::precede_0);
        }

        // specialization for quantity precedence
        void increment_dispatch(
                bool /* is_captured_king */, 
                Int2Type<rules::precede_N>
        )
        {
                BOOST_STATIC_ASSERT(rules::precedence_type<Rules>::value == rules::precede_N);
                current_.increment();
        }

        // specialization for quality precedence
        void increment_dispatch(
                bool is_captured_king, 
                Int2Type<rules::precede_Q>
        )
        {
                BOOST_STATIC_ASSERT(rules::precedence_type<Rules>::value == rules::precede_Q);
                current_.increment(is_captured_king);
        }

        void decrement(bool is_captured_king)
        {
                // tag dispatching on the type of capture precedence
                decrement_dispatch(
                        is_captured_king,
                        Int2Type<rules::precedence_type<Rules>::value>()
                );
        }

        // specialization for no capture precedence
        void decrement_dispatch(
                bool /* is_captured_king */, 
                Int2Type<rules::precede_0>
        )
        {
                BOOST_STATIC_ASSERT(rules::precedence_type<Rules>::value == rules::precede_0);
        }

        // specialization for quantity precedence
        void decrement_dispatch(
                bool /* is_captured_king */, 
                Int2Type<rules::precede_N>
        )
        {
                BOOST_STATIC_ASSERT(rules::precedence_type<Rules>::value == rules::precede_N);
                current_.decrement();
        }

        // specialization for quality precedence
        void decrement_dispatch(
                bool is_captured_king, 
                Int2Type<rules::precede_Q>
        )
        {
                BOOST_STATIC_ASSERT(rules::precedence_type<Rules>::value == rules::precede_Q);
                current_.decrement(is_captured_king);
        }

        void unique_back() const // modifies Stack& moves_
        {
                if (std::find(moves_.begin(), moves_.end(), moves_.back()) != moves_.end() - 1)
                        moves_.pop_back();
        }

        // partial specialization for man captures that are unambiguous
        template<bool Color>
        void add_pawn_jump_dispatch(
                BitBoard dest_sq, 
                Int2Type<false>
        ) const // modifies Stack& moves_
        {
                moves_.push_back(
                        Move::create<Color, Rules>(
                                from_sq_ ^ dest_sq,
                                promotion_sq<Color, Board>(dest_sq),
                                captured_pieces(),
                                pawn_captured_kings()
                        )
                );
        }

        // partial specialization for man captures that can be ambiguous
        template<bool Color>
        void add_pawn_jump_dispatch(
                BitBoard dest_sq, 
                Int2Type<true>
        ) const // modifies Stack& moves_
        {
                const auto ambiguous = is_ambiguous();
                add_pawn_jump_dispatch<Color>(dest_sq, Int2Type<false>());
                if (ambiguous)
                        unique_back();
        }

        // partial specialization for kings that halt immediately if the final capture is a king,
        // and slide through otherwise
        template<bool Color, int Index>
        void add_king_jump_dispatch(
                BitBoard dest_sq, bool ambiguous, 
                Int2Type<rules::halt_1K>
        ) const // modifies Stack& moves_
        {
                if (king_targets_ & Pull<Board, Index>()(dest_sq))
                        add_king_jump_dispatch<Color, Index>(
                                dest_sq, ambiguous, 
                                Int2Type<rules::halt_1>()
                        );
                else
                        add_king_jump_dispatch<Color, Index>(
                                dest_sq, ambiguous, 
                                Int2Type<rules::halt_N>()
                        );
        }

        // partial specialization for kings that halt immediately after the final capture
        template<bool Color, int Index>
        void add_king_jump_dispatch(
                BitBoard dest_sq, bool ambiguous, 
                Int2Type<rules::halt_1>
        ) const // modifies Stack& moves_
        {
                add_king_jump<Color>(dest_sq, ambiguous);
        }

        // partial specialization for kings that slide through after the final capture
        template<bool Color, int Index>
        void add_king_jump_dispatch(
                BitBoard dest_sq, bool ambiguous, 
                Int2Type<rules::halt_N>
        ) const // modifies Stack& moves_
        {
                BOOST_ASSERT(dest_sq & path());
                do {
                        add_king_jump<Color>(dest_sq, ambiguous);
                        PushAssign<Board, Index>()(dest_sq);
                } while (dest_sq & path());
        }

        template<bool Color>
        void add_king_jump(BitBoard dest_sq, bool ambiguous) const // modifies Stack& moves_
        {
                // tag dispatching on promotion condition
                add_king_jump_dispatch<Color>(
                        dest_sq,
                        Int2Type<rules::promotion_condition<Rules>::value>()
                );
                if (ambiguous)
                        unique_back();
        }

        // partial specialization for pawns that promote apres-fini
        template<bool Color>
        void add_king_jump_dispatch(
                BitBoard dest_sq, 
                Int2Type<rules::promote_af>
        ) const // modifies Stack& moves_
        {
                moves_.push_back(
                        Move::create<Color, Rules>(
                                from_sq_ ^ dest_sq,
                                captured_pieces(),
                                captured_kings()
                        )
                );
        }

        // partial specialization for pawns that promote en-passant
        template<bool Color>
        void add_king_jump_dispatch(
                BitBoard dest_sq, 
                Int2Type<rules::promote_ep>
        ) const // modifies Stack& moves_
        {
                if (!is_promotion())
                        add_king_jump_dispatch<Color>(
                                dest_sq, 
                                Int2Type<rules::promote_af>()
                        );
                else
                        moves_.push_back(
                                Move::create<Color, Rules>(
                                        from_sq_ ^ dest_sq,
                                        dest_sq,
                                        captured_pieces(),
                                        captured_kings()
                                )
                        );
        }

        // queries

        bool invariant() const
        {
                return (
                        bit::is_subset_of(remaining_targets_, initial_targets_) &&
                        !bit::is_multiple(from_sq_)
                );
        }

        bool is_captured_king(BitBoard target_sq) const
        {
                return !bit::is_zero(target_sq & king_targets_);
        }

        bool is_promotion() const
        {
                BOOST_STATIC_ASSERT(rules::promotion_condition<Rules>::value == rules::promote_ep);
                return current_.is_promotion();
        }

        bool is_ambiguous() const
        {
                return !moves_.empty() && is_large();
        }

        bool is_large() const
        {
                return count() >= rules::large_capture<Rules>::value;
        }

        int count() const
        {
                // tag dispatching on majority capture precedence
                return count_dispatch(
                        Int2Type<rules::is_majority_precedence<Rules>::value>()
                );
        }

        // specialization for no majority capture precedence
        int count_dispatch(
                Int2Type<false>
        ) const
        {
                BOOST_STATIC_ASSERT(!rules::is_majority_precedence<Rules>::value);
                return bit::count(captured_pieces());
        }

        // specialization for majority capture precedence
        int count_dispatch(
                Int2Type<true>
        ) const
        {
                BOOST_STATIC_ASSERT(rules::is_majority_precedence<Rules>::value);
                return current_.count();
        }

        BitBoard captured_pieces() const
        {
                return initial_targets_ ^ remaining_targets_;
        }

        BitBoard captured_kings() const
        {
                return captured_pieces() & king_targets_;
        }

        BitBoard pawn_captured_kings() const
        {
                // tag dispatching on whether pawns can capture kings
                return pawn_captured_kings_dispatch(
                        Int2Type<rules::is_pawns_jump_kings<Rules>::value>()
                );
        }

        // specialization for pawns that cannot capture kings
        BitBoard pawn_captured_kings_dispatch(
                Int2Type<false>
        ) const
        {
                BOOST_STATIC_ASSERT(!rules::is_pawns_jump_kings<Rules>::value);
                return BitBoard(0);
        }

        // specialization for pawns that can capture kings
        BitBoard pawn_captured_kings_dispatch(
                Int2Type<true>
        ) const
        {
                BOOST_STATIC_ASSERT(rules::is_pawns_jump_kings<Rules>::value);
                return captured_kings();
        }

        // representation

        const BitBoard king_targets_;
        BitBoard initial_targets_;
        BitBoard remaining_targets_;
        BitBoard not_occupied_;
        BitBoard from_sq_;
        Value<Rules> current_;
        Value<Rules> best_;
        Stack& moves_;
};

}       // namespace capture
}       // namespace dctl
