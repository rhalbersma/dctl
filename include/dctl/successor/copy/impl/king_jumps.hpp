#pragma once
#include <type_traits>                                  // false_type, true_type
#include <boost/assert.hpp>                             // BOOST_ASSERT
#include <boost/utility.hpp>                            // noncopyable
#include <dctl/successor/copy/impl/primary_fwd.hpp>     // copy (primary template)
#include <dctl/successor/material/king.hpp>             // king
#include <dctl/successor/propagate/jumps.hpp>           // Propagate
#include <dctl/successor/select/jumps.hpp>              // jumps

#include <dctl/angle/degrees.hpp>                       // Degrees
#include <dctl/angle/transform.hpp>                     // rotate
#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/iterator.hpp>                      // Next
#include <dctl/rules/traits.hpp>                        // traits
#include <dctl/utility/int.hpp>                         // BitBoard, BitIndex
#include <dctl/utility/algorithm.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for king jumps generation
template<bool Color, typename Position, typename Vector>
struct copy<Color, material::king, select::jumps, Position, Vector>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef typename Vector::value_type Move;
        typedef board::Compass<Color, Board> Compass;
        typedef Propagate<select::jumps, Position> State;

        // representation

        State& capture_;
        Vector& moves_;

public:
        // structors

        explicit copy(State& c, Vector& m)
        :
                capture_(c),
                moves_(m)
        {}

        // function call operators

        void operator()(BitBoard active_kings) const
        {
                // tag dispatching on relative king jump precedence
                if (active_kings)
                        select_dispatch(active_kings, rules::is_relative_king_precedence<Rules>());
        }

        template<typename Direction>
        bool promote_en_passant(BitIndex jumper) const
        {
                BOOST_ASSERT((is_promotion_sq<Color, Board>(jumper)));
                return find_next<Direction>(jumper);
        }

private:
        // overload for no relative king jump precedence
        void select_dispatch(BitBoard active_kings, std::false_type) const
        {
                serialize(active_kings);
        }

        // overload for relative king jump precedence
        void select_dispatch(BitBoard active_kings, std::true_type) const
        {
                capture_.toggle_with_king();
                serialize(active_kings);
                capture_.toggle_with_king();
        }

        void serialize(BitBoard active_kings) const
        {
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        find(bit::first::equal(active_kings));
                        bit::first::clear(active_kings);
                } while (active_kings);
        }

        void find(BitIndex jumper) const
        {
                capture_.launch(jumper);
                branch(jumper);
                capture_.finish(jumper);
        }

        void branch(BitIndex jumper) const
        {
                // tag dispatching on king jump directions
                branch_dispatch(jumper, rules::directions_king_jump<Rules>());
        }

        // overload for kings that capture in the 8 diagonal and orthogonal directions
        void branch_dispatch(BitIndex jumper, rules::directions::all) const
        {
                branch_dispatch(jumper, rules::directions::diag());
                branch_dispatch(jumper, rules::directions::orth());
        }

        // overload for kings that capture in the 4 diagonal directions
        void branch_dispatch(BitIndex jumper, rules::directions::diag) const
        {
                find_first<typename Compass::left_up   >(jumper);
                find_first<typename Compass::right_up  >(jumper);
                find_first<typename Compass::left_down >(jumper);
                find_first<typename Compass::right_down>(jumper);
        }

        // overload for kings that capture in the 4 orthogonal directions
        void branch_dispatch(BitIndex jumper, rules::directions::orth) const
        {
                find_first<typename Compass::left >(jumper);
                find_first<typename Compass::right>(jumper);
                find_first<typename Compass::up   >(jumper);
                find_first<typename Compass::down >(jumper);
        }

        template<typename Direction>
        void find_first(BitIndex jumper) const
        {
                slide<Direction>(jumper, capture_.template path<Direction>());
                if (bit::is_element(jumper, capture_.template targets_with_king<Direction>())) {
                        capture_.make(jumper);
                        precedence<Direction>(jumper);  // recursively find more jumps
                        capture_.undo(jumper);
                }
        }

        template<typename Direction>
        void precedence(BitIndex jumper) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch<Direction>(jumper, rules::is_precedence<Rules>());
        }

        // overload for no majority precedence
        template<typename Direction>
        void precedence_dispatch(BitIndex jumper, std::false_type) const
        {
                Increment<Board, Direction>()(jumper);
                if (!find_next<Direction>(jumper))
                        add_king_jump<Direction>(jumper);
        }

        // overload for majority precedence
        template<typename Direction>
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
                        add_king_jump<Direction>(jumper);
                }
        }

        template<typename Direction>
        bool find_next(BitIndex jumper) const
        {
                // tag dispatching on king jump direction reversal
                return find_next_dispatch<Direction>(jumper, rules::is_jump_direction_reversal<Rules>());
        }

        // overload for kings that cannot reverse their capture direction
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, std::false_type) const
        {
                return land<Direction>(jumper);
        }

        // overload for kings that can reverse their capture direction
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, std::true_type) const
        {
                return land<Direction>(jumper) | reverse<Direction>(jumper);
        }

        template<typename Direction>
        bool reverse(BitIndex jumper) const
        {
                return scan< typename mpl::lazy::rotate< Direction, angle::D180 >::type >(jumper);
        }

        template<typename Direction>
        bool land(BitIndex jumper) const
        {
                // tag dispatching on king jump landing range after intermediate captures
                return land_dispatch<Direction>(jumper, rules::range_land<Rules>());
        }

        // overload for kings that land immediately if the intermediate capture is a king, and slide through otherwise
        template<typename Direction>
        void land_dispatch(BitIndex jumper, rules::range::distance_1K) const
        {
                if (capture_.is_king(Prev<Board, Direction>()(jumper)))
                        land_dispatch<Direction>(jumper, rules::range::distance_1());
                else
                        land_dispatch<Direction>(jumper, rules::range::distance_N());
        }

        // overload for kings that can only land on the immediately adjacent square
        template<typename Direction>
        bool land_dispatch(BitIndex jumper, rules::range::distance_1) const
        {
                return turn<Direction>(jumper) | scan<Direction>(jumper);
        }

        // overload for kings that can land on any square along the current direction
        template<typename Direction>
        bool land_dispatch(BitIndex jumper, rules::range::distance_N) const
        {
                // NOTE: capture_.template path<Direction>() would be an ERROR here
                // because we need all landing squares rather than the directional launching squares subset
                BOOST_ASSERT(bit::is_element(jumper, capture_.path()));
                auto found_next = false;
                do {
                        found_next |= turn<Direction>(jumper);
                        Increment<Board, Direction>()(jumper);
                } while (bit::is_element(jumper, capture_.path()));
                return found_next |= jump<Direction>(jumper);
        }

        template<typename Direction>
        bool turn(BitIndex jumper) const
        {
                // tag dispatching on king turn directions
                return turn_dispatch<Direction>(jumper, rules::directions_king_turn<Rules>());
        }

        // overload for turns in all the 6 non-parallel diagonal and orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::all) const
        {
                return (
                        turn_dispatch<Direction>(jumper, rules::directions::diag()) |
                        turn_dispatch<Direction>(jumper, rules::directions::orth())
                );
        }

        // overload for turns in the 2 sideways directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::diag) const
        {
                return (
                        scan< typename mpl::lazy::rotate< Direction, angle::R090 >::type >(jumper) |
                        scan< typename mpl::lazy::rotate< Direction, angle::L090 >::type >(jumper)
                );
        }

        // overload for turns in the remaining 4 diagonal or orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::orth) const
        {
                return (
                        scan< typename mpl::lazy::rotate< Direction, angle::R045 >::type >(jumper) |
                        scan< typename mpl::lazy::rotate< Direction, angle::L045 >::type >(jumper) |
                        scan< typename mpl::lazy::rotate< Direction, angle::R135 >::type >(jumper) |
                        scan< typename mpl::lazy::rotate< Direction, angle::L135 >::type >(jumper)
                );
        }

        template<typename Direction>
        bool scan(BitIndex jumper) const
        {
                slide<Direction>(jumper, capture_.template path<Direction>());
                return jump<Direction>(jumper);
        }

        template<typename Direction>
        void slide(BitIndex& jumper, BitBoard path) const
        {
                // tag dispatching on king range
                slide_dispatch<Direction>(jumper, path, rules::range_king<Rules>());
        }

        // overload for short ranged kings
        template<typename Direction>
        void slide_dispatch(BitIndex& jumper, BitBoard /* path */, rules::range::distance_1) const
        {
                Increment<Board, Direction>()(jumper);
        }

        // overload for long ranged kings
        template<typename Direction>
        void slide_dispatch(BitIndex& jumper, BitBoard path, rules::range::distance_N) const
        {
                do Increment<Board, Direction>()(jumper); while (bit::is_element(jumper, path));
        }

        template<typename Direction>
        bool jump(BitIndex jumper) const
        {
                if (!bit::is_element(jumper, capture_.template targets_with_king<Direction>()))
                        return false;

                capture_.make(jumper);
                precedence<Direction>(jumper);  // recursively find more jumps
                capture_.undo(jumper);
                return true;
        }

        template<typename Direction>
        void add_king_jump(BitIndex dest_sq) const
        {
                auto const check_duplicate = rules::is_remove_duplicates<Rules>::value && capture_.is_potential_duplicate(moves_);

                // tag dispatching on king halt after final capture
                add_king_jump_dispatch<Direction>(dest_sq, check_duplicate, rules::range_halt<Rules>());
        }

        // overload for kings that halt immediately if the final capture is a king, and slide through otherwise
        template<typename Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, bool check_duplicate, rules::range::distance_1K) const
        {
                if (capture_.is_king(Prev<Board, Direction>()(dest_sq)))
                        add_king_jump_dispatch<Direction>(dest_sq, check_duplicate, rules::range::distance_1());
                else
                        add_king_jump_dispatch<Direction>(dest_sq, check_duplicate, rules::range::distance_N());
        }

        // overload for kings that halt immediately after the final capture
        template<typename Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, bool check_duplicate, rules::range::distance_1) const
        {
                add_king_jump(dest_sq, check_duplicate);
        }

        // overload for kings that slide through after the final capture
        template<typename Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, bool check_duplicate, rules::range::distance_N) const
        {
                // NOTE: capture_.template path<Direction>() would be an ERROR here
                // because we need all halting squares rather than the directional launching squares subset
                BOOST_ASSERT(bit::is_element(dest_sq, capture_.path()));
                do {
                        add_king_jump(dest_sq, check_duplicate);
                        Increment<Board, Direction>()(dest_sq);
                } while (bit::is_element(dest_sq, capture_.path()));
        }

        void add_king_jump(BitIndex dest_sq, bool check_duplicate) const
        {
                // tag dispatching on promotion condition
                add_king_jump_dispatch(dest_sq, rules::phase_promotion<Rules>());
                if (check_duplicate && algorithm::is_duplicate_back(moves_))
                        moves_.pop_back();
        }

        // overload for pawns that promote apres-fini
        void add_king_jump_dispatch(BitIndex dest_sq, rules::phase::apres_fini) const
        {
                capture_.template add_king_jump<Color>(dest_sq, moves_);
        }

        // overload for pawns that promote en-passant
        void add_king_jump_dispatch(BitIndex dest_sq, rules::phase::en_passant) const
        {
                if (!capture_.is_promotion())
                        capture_.template add_king_jump<Color>(dest_sq, moves_);
                else
                        capture_.template add_pawn_jump<Color, with::king>(dest_sq, moves_);
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
