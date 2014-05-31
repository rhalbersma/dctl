#pragma once
#include <dctl/angle.hpp>               // Angle, is_orthogonal
#include <dctl/bit.hpp>
#include <dctl/position/promotion.hpp>
#include <dctl/position/unary_projections.hpp>
#include <dctl/pieces/pieces.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/rules/types.hpp>
#include <dctl/successor/propagate_fwd.hpp>
#include <dctl/successor/select/jump.hpp>
#include <dctl/successor/value.hpp>
#include <dctl/utility/total_order.hpp>
#include <dctl/ray.hpp>
#include <dctl/wave/iterator.hpp>
#include <dctl/board/mask.hpp>
#include <boost/mpl/assert.hpp>
#include <cassert>                      // assert
#include <iterator>                     // begin, end, prev
#include <type_traits>                  // integral_constant, is_same, false_type, true_type

namespace dctl {
namespace successor {
namespace with {

struct king {};
struct pawn {};

}       // namespace with

template<class Position>
struct Propagate<select::jump, Position>
{
public:
        // structors

        explicit Propagate(Position const& p)
        :
                king_targets_(passive_kings(p)),
                initial_targets_(passive_pieces(p)),
                remaining_targets_(initial_targets_),
                not_occupied_(p.not_occupied())
        {
                assert(invariant());
        }

        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Set = typename Board::set_type;

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
                // tag dispatching on jump removal
                make_dispatch(sq, rules::is_en_passant_jump_removal_t<Rules>{});
                assert(invariant());
        }

        void undo(int sq)
        {
                // tag dispatching on jump removal
                undo_dispatch(sq, rules::is_en_passant_jump_removal_t<Rules>{});
                assert(invariant());
        }

        void toggle_with_king()
        {
                static_assert(rules::is_relative_king_jump_precedence_t<Rules>::value, "");
                current_.toggle_with_king();
        }

        void toggle_king_targets()
        {
                static_assert(!rules::can_jump<Rules, pieces::pawn, pieces::king>::value, "");
                initial_targets_ = remaining_targets_ ^= king_targets_;
        }

        void toggle_promotion()
        {
                BOOST_MPL_ASSERT((rules::is_en_passant_promotion_t<Rules>));
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
                moves.emplace_back(
                        captured_pieces(),
                        captured_kings(with::king{}),
                        from_sq_,
                        dest_sq,
                        Color
                );
        }

        template<bool Color, class WithPiece, class Sequence>
        void add_pawn_jump(int dest_sq, Sequence& moves) const
        {
                moves.emplace_back(
                        captured_pieces(),
                        captured_kings(WithPiece{}),
                        from_sq_,
                        dest_sq,
                        Color,
                        is_promotion<Color>(dest_sq, WithPiece{})
                );
        }

        // queries

        template<int Direction>
        auto targets_with_king() const
        {
                return remaining_targets<Direction>() & *std::prev(along_wave<Direction>(path()));
        }

        template<class Iterator>
        auto targets_with_king(Iterator it) const
        {
                return targets_with_king<ray::direction<Iterator>::value>().test(*it);
        }

        template<int Direction>
        auto targets_with_pawn() const
        {
                return remaining_targets_ & *std::prev(along_wave<Direction>(path()));
        }

        template<class Iterator>
        auto targets_with_pawn(Iterator it) const
        {
                return targets_with_pawn<ray::direction<Iterator>::value>().test(*it);
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
                auto constexpr jump_start = board::JumpStart<Board>::mask(Angle{Direction});
                return set_intersection(path(), jump_start);
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
                BOOST_MPL_ASSERT((rules::is_en_passant_promotion_t<Rules>));
                return current_.is_promotion();
        }

private:
        // modifiers

        // overload for apres-fini jump removal
        void make_dispatch(int sq, std::false_type)
        {
                make_impl(sq);
        }

        // overload for en-passant jump removal
        void make_dispatch(int sq, std::true_type)
        {
                not_occupied_.set(sq);
                make_impl(sq);
        }

        void make_impl(int sq)
        {
                remaining_targets_.reset(sq);
                increment(is_king(sq));
        }

        // overload for apres-fini jump removal
        void undo_dispatch(int sq, std::false_type)
        {
                undo_impl(sq);
        }

        // overload for en-passant jump removal
        void undo_dispatch(int sq, std::true_type)
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
                return set_intersection(remaining_targets_, king_targets_);
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
        static auto is_promotion(int dest_sq, with::pawn)
        {
                return dctl::is_promotion<Color, Board>(dest_sq);
        }

        // overload for pawn jumps with an en-passant promotion
        template<bool Color>
        static auto is_promotion(int /* dest_sq */, with::king)
        {
                return true;
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
                return Set{};
        }

        auto captured_kings(with::king) const
        {
                return set_intersection(captured_pieces(), king_targets_);
        }

        auto captured_pieces() const
        {
                return set_symmetric_difference(initial_targets_, remaining_targets_);
        }

        template<int Direction>
        static wave::Iterator<Board, Direction> along_wave(Set const& s)
        {
                return wave::make_iterator<Board, Direction>(s);
        }

        // contracts

        auto invariant() const
        {
                return set_includes(initial_targets_, remaining_targets_);
        }

        // representation

        Set const king_targets_;
        Set initial_targets_;
        Set remaining_targets_;
        Set not_occupied_;
        int from_sq_{-1};
        Value<Rules> current_{};
        Value<Rules> best_{};
};

}       // namespace successor
}       // namespace dctl
