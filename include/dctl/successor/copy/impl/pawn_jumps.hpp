#pragma once
#include <type_traits>                                  // false_type, true_type
#include <boost/assert.hpp>                             // BOOST_ASSERT
#include <boost/utility.hpp>                            // noncopyable
#include <dctl/successor/copy/impl/primary_fwd.hpp>     // copy (primary template)
#include <dctl/successor/copy/impl/king_jumps.hpp>      // promote_en_passant
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/pieces/pawn.hpp>           // pawn
#include <dctl/pieces/king.hpp>


#include <dctl/angle/degrees.hpp>
#include <dctl/angle/transform.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/angle/compass.hpp>
#include <dctl/board/iterator.hpp>
#include <dctl/node/promotion.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>
#include <dctl/utility/algorithm.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for pawn jumps generation
template<bool Color, class Position, class Sequence>
struct copy<Color, pieces::pawn, select::jumps, Position, Sequence>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        using KingJumps = copy<Color, pieces::king, select::jumps, Position, Sequence>;
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Compass = angle::Compass<Color, Board>;
        using State = Propagate<select::jumps, Position>;

        // representation

        State& capture_;
        Sequence& moves_;

public:
        // structors

        explicit copy(State& c, Sequence& m)
        :
                capture_(c),
                moves_(m)
        {}

        // function call operators

        void operator()(BitBoard active_pawns) const
        {
                // tag dispatching on whether pawns can capture kings
                if (active_pawns)
                        select_dispatch(active_pawns, rules::can_jump<Rules, pieces::pawn, pieces::king>());
        }

private:
        // overload for pawns that can capture kings
        void select_dispatch(BitBoard active_pawns, std::true_type) const
        {
                branch(active_pawns);
        }

        // overload for pawns that cannot capture kings
        void select_dispatch(BitBoard active_pawns, std::false_type) const
        {
                capture_.toggle_king_targets();
                branch(active_pawns);
                capture_.toggle_king_targets();
        }

        void branch(BitBoard active_pawns) const
        {
                // tag dispatching on pawn jump directions
                branch_dispatch(active_pawns, rules::directions::pawn_jump<Rules>());
        }

        // overload for pawns that jump in the 8 diagonal and orthogonal directions
        void branch_dispatch(BitBoard active_pawns, rules::directions::all) const
        {
                branch_dispatch(active_pawns, rules::directions::diag());
                branch_dispatch(active_pawns, rules::directions::orth());
        }

        // overload for pawns that jump in the 4 diagonal directions
        void branch_dispatch(BitBoard active_pawns, rules::directions::diag) const
        {
                branch_dispatch(active_pawns, rules::directions::up  ());
                branch_dispatch(active_pawns, rules::directions::down());
        }

        // overload for pawns that jump in the 2 forward diagonal directions
        void branch_dispatch(BitBoard active_pawns, rules::directions::up) const
        {
                serialize< Compass::left_up  >(active_pawns);
                serialize< Compass::right_up >(active_pawns);
        }

        // overload for pawns that jump in the 2 backward diagonal directions
        void branch_dispatch(BitBoard active_pawns, rules::directions::down) const
        {
                serialize< Compass::left_down  >(active_pawns);
                serialize< Compass::right_down >(active_pawns);
        }

        // overload for pawns that jump in the 4 orthogonal directions
        void branch_dispatch(BitBoard active_pawns, rules::directions::orth) const
        {
                serialize< Compass::left  >(active_pawns);
                serialize< Compass::right >(active_pawns);
                serialize< Compass::up    >(active_pawns);
                serialize< Compass::down  >(active_pawns);
        }

        template<int Direction>
        void serialize(BitBoard active_pawns) const
        {
                for (
                        active_pawns &= Prev<Board, Direction>()(capture_.template targets_with_pawn<Direction>());
                        active_pawns;
                        bit::pop_front(active_pawns)
                )
                        find<Direction>(bit::minimal_element(active_pawns));
        }

        template<int Direction>
        void find(BitIndex jumper) const
        {
                capture_.launch(jumper);
                find_first<Direction>(jumper);
                capture_.finish(jumper);
        }

        template<int Direction>
        void find_first(BitIndex jumper) const
        {
                Increment<Board, Direction>()(jumper);
                BOOST_ASSERT(bit::is_element(jumper, capture_.template targets_with_pawn<Direction>()));
                capture_.make(jumper);
                precedence<Direction>(jumper);  // recursively find more jumps
                capture_.undo(jumper);
        }

        template<int Direction>
        void precedence(BitIndex jumper) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch<Direction>(jumper, typename rules::is_precedence<Rules>());
        }

        // overload for no majority precedence
        template<int Direction>
        void precedence_dispatch(BitIndex jumper, std::false_type) const
        {
                Increment<Board, Direction>()(jumper);
                if (!find_next<Direction>(jumper))
                        add_pawn_jump(jumper);
        }

        // overload for majority precedence
        template<int Direction>
        void precedence_dispatch(BitIndex jumper, std::true_type) const
        {
                Increment<Board, Direction>()(jumper);
                if (
                        !find_next<Direction>(jumper) &&
                        capture_.greater_equal()
                ) {
                        if (capture_.not_equal_to()) {
                                capture_.improve();
                                moves_.clear();
                        }
                        add_pawn_jump(jumper);
                }
        }

        template<int Direction>
        bool find_next(BitIndex jumper) const
        {
                // tag dispatching on promotion condition
                return find_next_dispatch<Direction>(jumper, rules::phase::promotion<Rules>());
        }

        // overload for pawns that promote apres-fini
        template<int Direction>
        bool find_next_dispatch(BitIndex jumper, rules::phase::apres_fini) const
        {
                return find_next_impl<Direction>(jumper);
        }

        // overload for pawns that promote en-passant
        template<int Direction>
        bool find_next_dispatch(BitIndex jumper, rules::phase::en_passant) const
        {
                return (!is_promotion_sq<Color, Board>(jumper))?
                        find_next_impl<Direction>(jumper) :
                        promote_en_passant<Direction>(jumper)
                ;
        }

        template<int Direction>
        bool find_next_impl(BitIndex jumper) const
        {
                return turn<Direction>(jumper) | scan<Direction>(jumper);
        }

        template<int Direction>
        bool promote_en_passant(BitIndex jumper) const
        {
                // tag dispatching on whether pawns can capture kings
                return promote_en_passant_dispatch<Direction>(jumper, rules::can_jump<Rules, pieces::pawn, pieces::king>());
        }

        // overload for pawns that can capture kings
        template<int Direction>
        bool promote_en_passant_dispatch(BitIndex jumper, std::true_type) const
        {
                capture_.toggle_promotion();
                auto const found_next = KingJumps(capture_, moves_).template promote_en_passant<Direction>(jumper);
                capture_.toggle_promotion();
                return found_next;
        }

        // overload for pawns that cannot capture kings
        template<int Direction>
        bool promote_en_passant_dispatch(BitIndex jumper, std::false_type) const
        {
                capture_.toggle_promotion();    // no longer a pawn
                capture_.set_king_targets();    // can now capture kings
                auto const found_next = KingJumps(capture_, moves_).template promote_en_passant<Direction>(jumper);
                capture_.clear_king_targets();  // can no longer capture kings
                capture_.toggle_promotion();    // now a pawn again
                return found_next;
        }

        template<int Direction>
        bool turn(BitIndex jumper) const
        {
                // tag dispatching on pawn turn directions
                return turn_dispatch<Direction>(jumper, rules::directions::pawn_turn<Rules>());
        }

        // overload for pawns that turn in all the 6 non-parallel diagonal and orthogonal directions
        template<int Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::all) const
        {
                return (
                        turn_dispatch<Direction>(jumper, rules::directions::diag()) |
                        turn_dispatch<Direction>(jumper, rules::directions::orth())
                );
        }

        // overload for pawns that turn in the 2 sideways directions
        template<int Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::diag) const
        {
                return (
                        scan< angle::rotate(Direction, angle::R090) >(jumper) |
                        scan< angle::rotate(Direction, angle::L090) >(jumper)
                );
        }

        // overload for pawns that turn in the 1 mirrored forward direction
        template<int Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::up) const
        {
                return scan< angle::mirror(Direction, Compass::up) >(jumper);
        }

        // overload for pawns that turn in the 1 mirrored backward direction
        template<int Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::down) const
        {
                return scan< angle::mirror(Direction, Compass::down) >(jumper);
        }

        // overload for pawns that turn in the remaining 4 diagonal or orthogonal directions
        template<int Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::orth) const
        {
                return (
                        scan< angle::rotate(Direction, angle::R045) >(jumper) |
                        scan< angle::rotate(Direction, angle::L045) >(jumper) |
                        scan< angle::rotate(Direction, angle::R135) >(jumper) |
                        scan< angle::rotate(Direction, angle::L135) >(jumper)
                );
        }

        template<int Direction>
        bool scan(BitIndex jumper) const
        {
                Increment<Board, Direction>()(jumper);
                return jump<Direction>(jumper);
        }

        template<int Direction>
        bool jump(BitIndex jumper) const
        {
                if (!bit::is_element(jumper, capture_.template targets_with_pawn<Direction>()))
                        return false;

                capture_.make(jumper);
                precedence<Direction>(jumper);  // recursively find more jumps
                capture_.undo(jumper);
                return true;
        }

        void add_pawn_jump(BitIndex dest_sq) const
        {
                // tag dispatching on ambiguity of pawn jumps
                add_pawn_jump_dispatch(dest_sq, rules::is_unambiguous_pawn_jump<Rules>());
        }

        // overload for pawn jumps that are always unambiguous
        void add_pawn_jump_dispatch(BitIndex dest_sq, std::true_type) const
        {
                capture_.template add_pawn_jump<Color, with::pawn>(dest_sq, moves_);
        }

        // overload for pawn jumps that are potentially ambiguous
        void add_pawn_jump_dispatch(BitIndex dest_sq, std::false_type) const
        {
                auto const check_duplicate = rules::is_remove_duplicates<Rules>::value && capture_.is_potential_duplicate(moves_);
                capture_.template add_pawn_jump<Color, with::pawn>(dest_sq, moves_);
                if (check_duplicate && algorithm::is_duplicate_back(moves_))
                        moves_.pop_back();
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
