#pragma once
#include <iterator>                     // begin, end
#include <type_traits>                  // integral_constant, is_same, false_type, true_type
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/mpl/identity.hpp>       // identity
#include <boost/mpl/logical.hpp>        // and
#include <dctl/angle/traits.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/board/iterator.hpp>
#include <dctl/successor/value.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/promotion.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/successor/propagate_fwd.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/utility/int.hpp>
#include <dctl/utility/total_order.hpp>

namespace dctl {
namespace successor {
namespace with {

struct king {};
struct pawn {};

}       // namespace with

template<typename Position>
struct Propagate<Jumps, Position>
{
public:
        // structors

        explicit Propagate(Position const& p)
        :
                king_targets_(passive_kings(p)),
                initial_targets_(passive_pieces(p)),
                remaining_targets_(initial_targets_),
                not_occupied_(not_occupied(p)),
                from_sq_(0),
                current_(),
                best_()
        {
                BOOST_ASSERT(invariant());
        }

        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;

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
                make_dispatch(target_sq, typename rules::traits<Rules>::jump_removal());
                BOOST_ASSERT(invariant());
        }

        void undo(BitIndex target_sq)
        {
                // tag dispatching on capture removal
                undo_dispatch(target_sq, typename rules::traits<Rules>::jump_removal());
                BOOST_ASSERT(invariant());
        }

        void toggle_with_king()
        {
                BOOST_MPL_ASSERT((boost::mpl::identity<typename rules::traits<Rules>::is_relative_king_precedence>));
                current_.toggle_with_king();
        }

        void toggle_king_targets()
        {
                BOOST_MPL_ASSERT_NOT((boost::mpl::identity<typename rules::traits<Rules>::is_pawns_jump_kings>));
                initial_targets_ = remaining_targets_ ^= king_targets_;
        }

        void toggle_promotion()
        {
                BOOST_MPL_ASSERT((std::is_same<typename rules::traits<Rules>::pawn_promotion, rules::promotion::en_passant>));
                current_.toggle_promotion();
        }

        void improve()
        {
                BOOST_MPL_ASSERT((boost::mpl::identity<typename rules::traits<Rules>::is_precedence>));
                BOOST_ASSERT(best_ < current_);
                best_ = current_;
        }

        template<bool Color>
        void add_king_jump(BitIndex dest_sq, Vector<Move>& moves) const
        {
                moves.push_back(
                        Move::template create<Color, Rules>(
                                from_sq_ ^ dest_sq,
                                captured_pieces(),
                                captured_kings(with::king())
                        )
                );
        }

        template<bool Color, typename WithPiece>
        void add_pawn_jump(BitIndex dest_sq, Vector<Move>& moves) const
        {
                moves.push_back(
                        Move::template create<Color, Rules>(
                                from_sq_ ^ dest_sq,
                                promotion<Color>(dest_sq, WithPiece()),
                                captured_pieces(),
                                captured_kings(WithPiece())
                        )
                );
        }

        // queries

        template<typename Direction>
        BitBoard targets_with_king() const
        {
                return remaining_targets<Direction>() & Prev<Board, Direction>()(path());
        }

        template<typename Direction>
        BitBoard targets_with_pawn() const
        {
                return remaining_targets_ & Prev<Board, Direction>()(path());
        }

        BitBoard path() const
        {
                return not_occupied_;
        }

        template<typename Direction>
        BitBoard path() const
        {
                return path() & jump_start<Board, Direction>::value;
        }

        bool is_king(BitIndex target_sq) const
        {
                return bit::is_element(target_sq, king_targets_);
        }

        bool greater_equal() const
        {
                BOOST_MPL_ASSERT((boost::mpl::identity<typename rules::traits<Rules>::is_precedence>));
                BOOST_ASSERT(is_totally_ordered(best_, current_));
                return current_ >= best_;
        }

        bool not_equal_to() const
        {
                BOOST_MPL_ASSERT((boost::mpl::identity<typename rules::traits<Rules>::is_precedence>));
                BOOST_ASSERT(greater_equal());
                return current_ != best_;
        }

        bool is_potential_duplicate(Vector<Move> const& moves) const
        {
                return !moves.empty() && is_large();
        }

        bool is_large() const
        {
                return count() >= rules::traits<Rules>::large_jump::value;
        }

        bool is_promotion() const
        {
                BOOST_MPL_ASSERT((std::is_same<typename rules::traits<Rules>::pawn_promotion, rules::promotion::en_passant>));
                return current_.is_promotion();
        }

private:
        // modifiers

        // overload for apres-fini capture removal
        void make_dispatch(BitIndex target_sq, rules::removal::apres_fini)
        {
                make_impl(target_sq);
        }

        // overload for en-passant capture removal
        void make_dispatch(BitIndex target_sq, rules::removal::en_passant)
        {
                not_occupied_ ^= target_sq;
                make_impl(target_sq);
        }

        void make_impl(BitIndex target_sq)
        {
                remaining_targets_ ^= target_sq;
                increment(is_king(target_sq));
        }

        // overload for apres-fini capture removal
        void undo_dispatch(BitIndex target_sq, rules::removal::apres_fini)
        {
                undo_impl(target_sq);
        }

        // overload for en-passant capture removal
        void undo_dispatch(BitIndex target_sq, rules::removal::en_passant)
        {
                undo_impl(target_sq);
                not_occupied_ ^= target_sq;
        }

        void undo_impl(BitIndex target_sq)
        {
                decrement(is_king(target_sq));
                remaining_targets_ ^= target_sq;
        }

        void increment(bool is_king)
        {
                // tag dispatching on the type of capture precedence
                increment_dispatch(is_king, typename rules::traits<Rules>::jump_precedence());
        }

        // overload for no capture precedence
        void increment_dispatch(bool /* is_king */, rules::precedence::none)
        {
                // no-op
        }

        // overload for quantity precedence
        void increment_dispatch(bool /* is_king */, rules::precedence::quantity)
        {
                current_.increment();
        }

        // overload for quality precedence
        void increment_dispatch(bool is_king, rules::precedence::quality)
        {
                current_.increment(is_king);
        }

        void decrement(bool is_king)
        {
                // tag dispatching on the type of capture precedence
                decrement_dispatch(is_king, typename rules::traits<Rules>::jump_precedence());
        }

        // overload for no capture precedence
        void decrement_dispatch(bool /* is_king */, rules::precedence::none)
        {
                // no-op
        }

        // overload for quantity precedence
        void decrement_dispatch(bool /* is_king */, rules::precedence::quantity)
        {
                current_.decrement();
        }

        // overload for quality precedence
        void decrement_dispatch(bool is_king, rules::precedence::quality)
        {
                current_.decrement(is_king);
        }

        // queries

        bool invariant() const
        {
                return (
                        bit::is_subset_of(remaining_targets_, initial_targets_) &&
                        !bit::is_multiple(from_sq_)
                );
        }

        template<typename Direction>
        BitBoard remaining_targets() const
        {
        	// tag dispatching based on direction and king jump orthogonality
                return remaining_targets_dispatch(
                        std::integral_constant<
                                bool,
                                boost::mpl::and_<
                                        angle::lazy::is_orthogonal<Direction>,
                                        std::is_same<typename
                                                rules::traits<Rules>::king_jump_orthogonality,
                                                rules::orthogonality::relative
                                        >
                                >::value
                        >()
                );
        }

        // overload for diagonal direction or king jump
        BitBoard remaining_targets_dispatch(std::false_type) const
        {
                return remaining_targets_;
        }

        // overload for orthogonal direction and king jump
        BitBoard remaining_targets_dispatch(std::true_type) const
        {
                return remaining_targets_ & king_targets_;
        }

        int count() const
        {
                // tag dispatching on majority capture precedence
                return count_dispatch(typename rules::traits<Rules>::is_precedence());
        }

        // overload for no majority capture precedence
        int count_dispatch(std::false_type) const
        {
                return bit::count(captured_pieces());
        }

        // overload for majority capture precedence
        int count_dispatch(std::true_type) const
        {
                return current_.count();
        }

        // overload for pawn jumps without promotion
        template<bool Color>
        BitBoard promotion(BitIndex dest_sq, with::pawn) const
        {
                return promotion_sq<Color, Board>(dest_sq);
        }

        // overload for pawn jumps with an en-passant promotion
        template<bool Color>
        BitBoard promotion(BitIndex dest_sq, with::king) const
        {
                return dest_sq;
        }

        BitBoard captured_kings(with::pawn) const
        {
                // tag dispatching on whether pawns can capture kings
                return captured_kings_dispatch(typename rules::traits<Rules>::is_pawns_jump_kings());
        }

        // overload for pawns that can capture kings
        BitBoard captured_kings_dispatch(std::true_type) const
        {
                return captured_kings(with::king());
        }

        // overload for pawns that cannot capture kings
        BitBoard captured_kings_dispatch(std::false_type) const
        {
                return BitBoard(0);
        }

        BitBoard captured_kings(with::king) const
        {
                return captured_pieces() & king_targets_;
        }

        BitBoard captured_pieces() const
        {
                return initial_targets_ ^ remaining_targets_;
        }

        // representation

        BitBoard const king_targets_;
        BitBoard initial_targets_;
        BitBoard remaining_targets_;
        BitBoard not_occupied_;
        BitIndex from_sq_;
        Value<Rules> current_;
        Value<Rules> best_;
};

}       // namespace successor
}       // namespace dctl
