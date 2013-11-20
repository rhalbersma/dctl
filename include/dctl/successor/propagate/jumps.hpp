#pragma once
#include <cassert>                      // assert
#include <iterator>                     // begin, end
#include <type_traits>                  // integral_constant, is_same, false_type, true_type
#include <dctl/angle.hpp>               // Angle, is_orthogonal
#include <dctl/bit/bit.hpp>
#include <dctl/bit/algorithm.hpp>
#include <dctl/bit/bitboard.hpp>
#include <dctl/board/iterator.hpp>      // Prev
#include <dctl/node/material.hpp>
#include <dctl/node/promotion.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/pieces/pieces.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/rules/types.hpp>
#include <dctl/successor/propagate_fwd.hpp>
#include <dctl/successor/select/jumps.hpp>
#include <dctl/successor/value.hpp>
#include <dctl/utility/total_order.hpp>
#include <dctl/bit/bitboard.hpp>        // BitBoard, BitIndex
#include <dctl/ray/iterator.hpp>

#include <dctl/bit/bit_set.hpp>
#include <dctl/bit/algorithm.hpp>

namespace dctl {
namespace successor {
namespace with {

struct king {};
struct pawn {};

}       // namespace with

template<class Position>
struct Propagate<select::jumps, Position>
{
public:
        // structors

        explicit Propagate(Position const& p)
        :
                king_targets_(BitSet(passive_kings(p))),
                initial_targets_(BitSet(passive_pieces(p))),
                remaining_targets_(initial_targets_),
                not_occupied_(BitSet(not_occupied(p))),
                from_sq_(-1),
                current_(),
                best_()
        {
                assert(invariant());
        }

        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;

        using BitSet = bit::bit_set<int, uint64_t, 1>;

        // modifiers

        void launch(int sq)
        {
                from_sq_ = sq;
                not_occupied_.set(sq);
                assert(invariant());
        }

        void finish(int sq)
        {
                not_occupied_.reset(sq);
                from_sq_ = -1;
                assert(invariant());
        }

        void make(int sq)
        {
                // tag dispatching on capture removal
                make_dispatch(sq, rules::phase::capture<Rules>{});
                assert(invariant());
        }

        void undo(int sq)
        {
                // tag dispatching on capture removal
                undo_dispatch(sq, rules::phase::capture<Rules>{});
                assert(invariant());
        }

        void toggle_with_king()
        {
                static_assert(rules::precedence::is_relative_king<Rules>::value, "");
                current_.toggle_with_king();
        }

        void toggle_king_targets()
        {
                static_assert(!rules::can_jump<Rules, pieces::pawn, pieces::king>::value, "");
                initial_targets_ = remaining_targets_ ^= king_targets_;
        }

        void toggle_promotion()
        {
                static_assert(std::is_same<typename rules::phase::promotion<Rules>::type, rules::phase::en_passant>::value, "");
                current_.toggle_promotion();
        }

        void improve()
        {
                static_assert(rules::is_precedence<Rules>::value, "");
                assert(best_ < current_);
                best_ = current_;
        }

        template<bool Color, class Sequence>
        void add_king_jump(int dest_sq, Sequence& moves) const
        {
                using Move = typename Sequence::value_type;

                moves.push_back(
                        Move::template create<Color, Rules>(
                                {from_sq_, dest_sq},
                                captured_pieces(),
                                captured_kings(with::king())
                        )
                );
        }

        template<bool Color, class WithPiece, class Sequence>
        void add_pawn_jump(int dest_sq, Sequence& moves) const
        {
                using Move = typename Sequence::value_type;

                moves.push_back(
                        Move::template create<Color, Rules>(
                                {from_sq_, dest_sq},
                                promotion<Color>(dest_sq, WithPiece()),
                                captured_pieces(),
                                captured_kings(WithPiece())
                        )
                );
        }

        // queries

        template<int Direction>
        auto targets_with_king() const
        {
                return remaining_targets<Direction>() & Prev<Board, Direction>{}(path());
        }

        template<int Direction>
        auto targets_with_king(int sq) const
        {
                return targets_with_king<Direction>().test(sq);
        }

        template<int Direction>
        auto targets_with_pawn() const
        {
                return remaining_targets_ & Prev<Board, Direction>{}(path());
        }

        template<int Direction>
        auto targets_with_pawn(int sq) const
        {
                return targets_with_pawn<Direction>().test(sq);
        }

        auto path() const
        {
                return not_occupied_;
        }

        auto path(int sq) const
        {
                return not_occupied_.test(sq);
        }

        template<int Direction>
        auto path() const
        {
                return path() & BitSet(Board::jump_start(Angle{Direction}));
        }

        template<int Direction>
        auto path(int sq) const
        {
                return path<Direction>().test(sq);
        }

        auto is_king(int sq) const
        {
                return king_targets_.test(sq);
        }

        auto greater_equal() const
        {
                static_assert(rules::is_precedence<Rules>::value, "");
                assert(is_totally_ordered(best_, current_));
                return current_ >= best_;
        }

        auto not_equal_to() const
        {
                static_assert(rules::is_precedence<Rules>::value, "");
                assert(greater_equal());
                return current_ != best_;
        }

        template<class Sequence>
        auto is_potential_duplicate(Sequence const& moves) const
        {
                return !moves.empty() && is_large();
        }

        auto is_large() const
        {
                return size() >= rules::large_jump<Rules>::value;
        }

        auto is_promotion() const
        {
                static_assert(std::is_same<typename rules::phase::promotion<Rules>::type, rules::phase::en_passant>::value, "");
                return current_.is_promotion();
        }

private:
        // modifiers

        // overload for apres-fini capture removal
        void make_dispatch(int sq, rules::phase::apres_fini)
        {
                make_impl(sq);
        }

        // overload for en-passant capture removal
        void make_dispatch(int sq, rules::phase::en_passant)
        {
                not_occupied_.set(sq);
                make_impl(sq);
        }

        void make_impl(int sq)
        {
                remaining_targets_.reset(sq);
                increment(is_king(sq));
        }

        // overload for apres-fini capture removal
        void undo_dispatch(int sq, rules::phase::apres_fini)
        {
                undo_impl(sq);
        }

        // overload for en-passant capture removal
        void undo_dispatch(int sq, rules::phase::en_passant)
        {
                undo_impl(sq);
                not_occupied_.reset(sq);
        }

        void undo_impl(int sq)
        {
                decrement(is_king(sq));
                remaining_targets_.set(sq);
        }

        void increment(bool is_king)
        {
                // tag dispatching on the type of capture precedence
                increment_dispatch(is_king, rules::precedence::jump<Rules>{});
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
                decrement_dispatch(is_king, rules::precedence::jump<Rules>{});
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

        auto invariant() const
        {
                return (
                        bit::set_includes(initial_targets_, remaining_targets_)
                );
        }

        template<int Direction>
        auto remaining_targets() const
        {
        	// tag dispatching based on direction and king jump orthogonality
                return remaining_targets_dispatch(
                        std::integral_constant<
                                bool,
                                is_orthogonal(Angle{Direction}) &&
                                std::is_same< typename
                                        rules::orthogonality::king_jump<Rules>::type,
                                        rules::orthogonality::relative
                                >::value
                        >()
                );
        }

        // overload for diagonal direction or king jump
        auto remaining_targets_dispatch(std::false_type) const
        {
                return remaining_targets_;
        }

        // overload for orthogonal direction and king jump
        auto remaining_targets_dispatch(std::true_type) const
        {
                return remaining_targets_ & king_targets_;
        }

        auto size() const
        {
                // tag dispatching on majority capture precedence
                return size_dispatch(rules::is_precedence<Rules>{});
        }

        // overload for no majority capture precedence
        auto size_dispatch(std::false_type) const
        {
                return captured_pieces().size();
        }

        // overload for majority capture precedence
        auto size_dispatch(std::true_type) const
        {
                return current_.size();
        }

        // overload for pawn jumps without promotion
        template<bool Color>
        auto promotion(int dest_sq, with::pawn) const
        {
                return promotion_sq<Color, Board>(dest_sq);
        }

        // overload for pawn jumps with an en-passant promotion
        template<bool Color>
        auto promotion(int dest_sq, with::king) const
        {
                return BitBoard{1} << dest_sq;
        }

        auto captured_kings(with::pawn) const
        {
                // tag dispatching on whether pawns can capture kings
                return captured_kings_dispatch(rules::can_jump<Rules, pieces::pawn, pieces::king>());
        }

        // overload for pawns that can capture kings
        auto captured_kings_dispatch(std::true_type) const
        {
                return captured_kings(with::king());
        }

        // overload for pawns that cannot capture kings
        auto captured_kings_dispatch(std::false_type) const
        {
                return BitSet{};
        }

        auto captured_kings(with::king) const
        {
                return captured_pieces() & king_targets_;
        }

        auto captured_pieces() const
        {
                return initial_targets_ ^ remaining_targets_;
        }

        // representation

        BitSet const king_targets_;
        BitSet initial_targets_;
        BitSet remaining_targets_;
        BitSet not_occupied_;
        int from_sq_;
        Value<Rules> current_;
        Value<Rules> best_;
};

}       // namespace successor
}       // namespace dctl
