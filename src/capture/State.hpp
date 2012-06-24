#pragma once
#include <algorithm>                    // find
#include <iterator>                     // begin, end
#include <type_traits>                  // is_same
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/mpl/identity.hpp>       // identity
#include <boost/utility.hpp>            // noncopyable
#include "Value.hpp"
#include "../bit/Bit.hpp"
#include "../board/Shift.hpp"
#include "../node/Stack.hpp"
#include "../rules/Enum.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/total_order.hpp"

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
        explicit State(Position<Rules, Board> const& p, Stack& m)
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

        void launch(BitIndex jump_sq)
        {
                from_sq_ = jump_sq;
                not_occupied_ ^= jump_sq;
                BOOST_ASSERT(invariant());
        }

        void finish(BitIndex jump_sq)
        {
                not_occupied_ ^= jump_sq;
                from_sq_ = BitIndex(0);
                BOOST_ASSERT(invariant());
        }

        void make(BitIndex target_sq)
        {
                // tag dispatching on capture removal
                make_dispatch(
                        target_sq,
                        typename Rules::jump_removal()
                );
                BOOST_ASSERT(invariant());
        }

        void undo(BitIndex target_sq)
        {
                // tag dispatching on capture removal
                undo_dispatch(
                        target_sq,
                        typename Rules::jump_removal()
                );
                BOOST_ASSERT(invariant());
        }

        void toggle_with_king()
        {              
                BOOST_MPL_ASSERT((boost::mpl::identity<typename Rules::is_relative_king_precedence>));
                current_.toggle_with_king();
        }

        void toggle_king_targets()
        {
                BOOST_MPL_ASSERT_NOT((boost::mpl::identity<typename Rules::is_pawns_jump_kings>));
                initial_targets_ = remaining_targets_ ^= king_targets_;
        }

        void toggle_promotion()
        {
                BOOST_MPL_ASSERT((std::is_same<typename Rules::pawn_promotion, rules::promotion::en_passant>));
                current_.toggle_promotion();
        }

        void improve()
        {
                BOOST_ASSERT(Rules::is_majority_precedence::value);
                best_ = current_;
                moves_.clear();
        }

        template<bool Color>
        void add_pawn_jump(BitIndex dest_sq) const // modifies Stack& moves_
        {
                // tag dispatching on ambiguity of pawn captures
                add_pawn_jump_dispatch<Color>(
                        dest_sq,
                        typename Rules::is_ambiguous_pawn_jump()
                );
        }

        template<bool Color, typename Index>
        void add_king_jump(BitIndex dest_sq) const // modifies Stack& moves_
        {
                auto const ambiguous = is_ambiguous();

                // tag dispatching on king halt after final capture
                add_king_jump_dispatch<Color, Index>(
                        dest_sq, ambiguous,
                        typename Rules::halt_range()
                );
        }

        // queries

        template<typename Index>
        BitBoard targets() const
        {
                return remaining_targets_ & Pull<Board, Index>()(path());
        }

        template<typename Index>
        BitBoard path() const
        {
                return path() & Board::jump_start[Index::value];
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
                BitIndex target_sq, 
                rules::removal::apres_fini
        )
        {
                remaining_targets_ ^= target_sq;
                increment(is_captured_king(target_sq));
        }

        // specialization for en-passant capture removal
        void make_dispatch(
                BitIndex target_sq, 
                rules::removal::en_passant
        )
        {
                BOOST_MPL_ASSERT((std::is_same<typename Rules::jump_removal, rules::removal::en_passant>));
                not_occupied_ ^= target_sq;
                make_dispatch(
                        target_sq, 
                        rules::removal::apres_fini()
                );
        }

        // specialization for apres-fini capture removal
        void undo_dispatch(
                BitIndex target_sq, 
                rules::removal::apres_fini
        )
        {
                decrement(is_captured_king(target_sq));
                remaining_targets_ ^= target_sq;
        }

        // specialization for en-passant capture removal
        void undo_dispatch(
                BitIndex target_sq, 
                rules::removal::en_passant
        )
        {
                BOOST_MPL_ASSERT((std::is_same<typename Rules::jump_removal, rules::removal::en_passant>));
                undo_dispatch(
                        target_sq, 
                        rules::removal::apres_fini()
                );
                not_occupied_ ^= target_sq;
        }

        void increment(bool is_captured_king)
        {
                // tag dispatching on the type of capture precedence
                increment_dispatch(
                        is_captured_king,
                        typename Rules::jump_precedence()
                );
        }

        // specialization for no capture precedence
        void increment_dispatch(
                bool /* is_captured_king */, 
                rules::precedence::none
        )
        {
                BOOST_MPL_ASSERT((std::is_same<typename Rules::jump_precedence, rules::precedence::none>));
        }

        // specialization for quantity precedence
        void increment_dispatch(
                bool /* is_captured_king */, 
                rules::precedence::quantity
        )
        {
                BOOST_MPL_ASSERT((std::is_same<typename Rules::jump_precedence, rules::precedence::quantity>));
                current_.increment();
        }

        // specialization for quality precedence
        void increment_dispatch(
                bool is_captured_king, 
                rules::precedence::quality
        )
        {
                BOOST_MPL_ASSERT((std::is_same<typename Rules::jump_precedence, rules::precedence::quality>));
                current_.increment(is_captured_king);
        }

        void decrement(bool is_captured_king)
        {
                // tag dispatching on the type of capture precedence
                decrement_dispatch(
                        is_captured_king,
                        typename Rules::jump_precedence()
                );
        }

        // specialization for no capture precedence
        void decrement_dispatch(
                bool /* is_captured_king */, 
                rules::precedence::none
        )
        {
                BOOST_MPL_ASSERT((std::is_same<typename Rules::jump_precedence, rules::precedence::none>));
        }

        // specialization for quantity precedence
        void decrement_dispatch(
                bool /* is_captured_king */, 
                rules::precedence::quantity
        )
        {
                BOOST_MPL_ASSERT((std::is_same<typename Rules::jump_precedence, rules::precedence::quantity>));
                current_.decrement();
        }

        // specialization for quality precedence
        void decrement_dispatch(
                bool is_captured_king, 
                rules::precedence::quality
        )
        {
                BOOST_MPL_ASSERT((std::is_same<typename Rules::jump_precedence, rules::precedence::quality>));
                current_.decrement(is_captured_king);
        }

        void unique_back() const // modifies Stack& moves_
        {
                if (std::find(std::begin(moves_), std::end(moves_), moves_.back()) != std::end(moves_) - 1)
                        moves_.pop_back();
        }

        // partial specialization for pawn captures that are unambiguous
        template<bool Color>
        void add_pawn_jump_dispatch(
                BitIndex dest_sq, 
                boost::mpl::false_
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

        // partial specialization for pawn captures that can be ambiguous
        template<bool Color>
        void add_pawn_jump_dispatch(
                BitIndex dest_sq, 
                boost::mpl::true_
        ) const // modifies Stack& moves_
        {
                auto const ambiguous = is_ambiguous();
                add_pawn_jump_dispatch<Color>(dest_sq, boost::mpl::false_());
                if (ambiguous)
                        unique_back();
        }

        // partial specialization for kings that halt immediately if the final capture is a king,
        // and slide through otherwise
        template<bool Color, typename Index>
        void add_king_jump_dispatch(
                BitIndex dest_sq, bool ambiguous, 
                rules::range::distance_1K
        ) const // modifies Stack& moves_
        {
                if (bit::is_element(Board::prev<Index>(dest_sq), king_targets_))
                        add_king_jump_dispatch<Color, Index>(
                                dest_sq, ambiguous, 
                                rules::range::distance_1()
                        );
                else
                        add_king_jump_dispatch<Color, Index>(
                                dest_sq, ambiguous, 
                                rules::range::distance_N()
                        );
        }

        // partial specialization for kings that halt immediately after the final capture
        template<bool Color, typename Index>
        void add_king_jump_dispatch(
                BitIndex dest_sq, bool ambiguous, 
                rules::range::distance_1
        ) const // modifies Stack& moves_
        {
                add_king_jump<Color>(dest_sq, ambiguous);
        }

        // partial specialization for kings that slide through after the final capture
        template<bool Color, typename Index>
        void add_king_jump_dispatch(
                BitIndex dest_sq, bool ambiguous, 
                rules::range::distance_N
        ) const // modifies Stack& moves_
        {
                BOOST_ASSERT(bit::is_element(dest_sq, path()));
                do {
                        add_king_jump<Color>(dest_sq, ambiguous);
                        Board::advance<Index>(dest_sq);
                } while (bit::is_element(dest_sq, path()));
        }

        template<bool Color>
        void add_king_jump(BitIndex dest_sq, bool ambiguous) const // modifies Stack& moves_
        {
                // tag dispatching on promotion condition
                add_king_jump_dispatch<Color>(
                        dest_sq,
                        typename Rules::pawn_promotion()
                );
                if (ambiguous)
                        unique_back();
        }

        // partial specialization for pawns that promote apres-fini
        template<bool Color>
        void add_king_jump_dispatch(
                BitIndex dest_sq, 
                rules::promotion::apres_fini
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
                BitIndex dest_sq, 
                rules::promotion::en_passant
        ) const // modifies Stack& moves_
        {
                if (!is_promotion())
                        add_king_jump_dispatch<Color>(
                                dest_sq, 
                                rules::promotion::apres_fini()
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

        bool is_captured_king(BitIndex target_sq) const
        {
                return bit::is_element(target_sq, king_targets_);
        }

        bool is_promotion() const
        {
                BOOST_MPL_ASSERT((std::is_same<typename Rules::pawn_promotion, rules::promotion::en_passant>));
                return current_.is_promotion();
        }

        bool is_ambiguous() const
        {
                return !moves_.empty() && is_large();
        }

        bool is_large() const
        {
                return count() >= Rules::large_jump::value;
        }

        int count() const
        {
                // tag dispatching on majority capture precedence
                return count_dispatch(
                        typename Rules::is_majority_precedence()
                );
        }

        // specialization for no majority capture precedence
        int count_dispatch(
                boost::mpl::false_
        ) const
        {
                BOOST_MPL_ASSERT_NOT((boost::mpl::identity<typename Rules::is_majority_precedence>));
                return bit::count(captured_pieces());
        }

        // specialization for majority capture precedence
        int count_dispatch(
                boost::mpl::true_
        ) const
        {
                BOOST_MPL_ASSERT((boost::mpl::identity<typename Rules::is_majority_precedence>));
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
                        typename Rules::is_pawns_jump_kings()
                );
        }

        // specialization for pawns that cannot capture kings
        BitBoard pawn_captured_kings_dispatch(
                boost::mpl::false_
        ) const
        {
                BOOST_MPL_ASSERT_NOT((boost::mpl::identity<typename Rules::is_pawns_jump_kings>));
                return BitBoard(0);
        }

        // specialization for pawns that can capture kings
        BitBoard pawn_captured_kings_dispatch(
                boost::mpl::true_
        ) const
        {
                BOOST_MPL_ASSERT((boost::mpl::identity<typename Rules::is_pawns_jump_kings>));
                return captured_kings();
        }

        // representation

        const BitBoard king_targets_;
        BitBoard initial_targets_;
        BitBoard remaining_targets_;
        BitBoard not_occupied_;
        BitIndex from_sq_;
        Value<Rules, Board> current_;
        Value<Rules, Board> best_;
        Stack& moves_;
};

}       // namespace capture
}       // namespace dctl
