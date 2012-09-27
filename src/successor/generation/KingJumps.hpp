#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/mpl/bool.hpp>           // false_, true_
#include "Generator_fwd.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Degrees.hpp"
#include "../../board/Shift.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps generation
template<bool Color, typename Position>
struct generator<Color, Material::king, Jumps, Position>
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;
        typedef capture::State<Position> State;

public:
        void operator()(Position const& p, Stack& moves) const
        {
                if (auto const active_kings = p.kings(Color)) {
                        State capture(p, moves);
                        select(active_kings, capture);
                }
        }

        void operator()(Position const& p, State& capture) const
        {
                if (auto const active_kings = p.kings(Color))
                        select(active_kings, capture);
        }

        template<typename Direction>
        bool promote_en_passant(BitIndex jumper, State& capture) const
        {
                BOOST_ASSERT((is_promotion_sq<Color, Board>(jumper)));
                return find_next<Direction>(jumper, capture);
        }

private:
        void select(BitBoard active_kings, State& capture) const
        {
                // tag dispatching on relative king jump precedence
                select_dispatch(active_kings, capture, typename Rules::is_relative_king_precedence());
        }

        // overload for no relative king jump precedence
        void select_dispatch(BitBoard active_kings, State& capture, boost::mpl::false_) const
        {
                serialize(active_kings, capture);
        }

        // overload for relative king jump precedence
        void select_dispatch(BitBoard active_kings, State& capture, boost::mpl::true_) const
        {
                capture.toggle_with_king();
                serialize(active_kings, capture);
                capture.toggle_with_king();
        }

        void serialize(BitBoard active_kings, State& capture) const
        {
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        find(bit::get_first(active_kings), capture);
                        bit::clear_first(active_kings);
                } while (active_kings);
        }

        void find(BitIndex jumper, State& capture) const
        {
                capture.launch(jumper);
                branch(jumper, capture);
                capture.finish(jumper);
        }

        void branch(BitIndex jumper, State& capture) const
        {
                // tag dispatching on king jump directions
                branch_dispatch(jumper, capture, typename Rules::king_jump_directions());
        }

        // overload for kings that capture in the 8 diagonal and orthogonal directions
        void branch_dispatch(BitIndex jumper, State& capture, rules::directions::all) const
        {
                branch_dispatch(jumper, capture, rules::directions::diag());
                branch_dispatch(jumper, capture, rules::directions::orth());
        }

        // overload for kings that capture in the 4 diagonal directions
        void branch_dispatch(BitIndex jumper, State& capture, rules::directions::diag) const
        {
                find_first<typename Compass::left_up   >(jumper, capture);
                find_first<typename Compass::right_up  >(jumper, capture);
                find_first<typename Compass::left_down >(jumper, capture);
                find_first<typename Compass::right_down>(jumper, capture);
        }

        // overload for kings that capture in the 4 orthogonal directions
        void branch_dispatch(BitIndex jumper, State& capture, rules::directions::orth) const
        {
                find_first<typename Compass::left >(jumper, capture);
                find_first<typename Compass::right>(jumper, capture);
                find_first<typename Compass::up   >(jumper, capture);
                find_first<typename Compass::down >(jumper, capture);
        }

        template<typename Direction>
        void find_first(BitIndex jumper, State& capture) const
        {
                slide<Direction>(jumper, capture.template path<Direction>());
                if (bit::is_element(jumper, capture.template targets<Direction>())) {
                        capture.make(jumper);
                        precedence<Direction>(jumper, capture); // recursively find more jumps
                        capture.undo(jumper);
                }
        }

        template<typename Direction>
        void precedence(BitIndex jumper, State& capture) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch<Direction>(jumper, capture, typename Rules::is_majority_precedence());
        }

        // overload for no majority precedence
        template<typename Direction>
        void precedence_dispatch(BitIndex jumper, State& capture, boost::mpl::false_) const
        {
                Board::advance<Direction>(jumper);
                if (!find_next<Direction>(jumper, capture))
                        add_king_jump<Direction>(jumper, capture);
        }

        // overload for majority precedence
        template<typename Direction>
        void precedence_dispatch(BitIndex jumper, State& capture, boost::mpl::true_) const
        {
                Board::advance<Direction>(jumper);
                if (
                        !find_next<Direction>(jumper, capture) &&
                        capture.greater_equal()
                ) {
                        if (capture.not_equal_to())
                                capture.improve();
                        add_king_jump<Direction>(jumper, capture);
                }
        }

        template<typename Direction>
        bool find_next(BitIndex jumper, State& capture) const
        {
                // tag dispatching on king jump direction reversal
                return find_next_dispatch<Direction>(jumper, capture, typename Rules::is_jump_direction_reversal());
        }

        // overload for kings that cannot reverse their capture direction
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, State& capture, boost::mpl::false_) const
        {
                return land<Direction>(jumper, capture);
        }

        // overload for kings that can reverse their capture direction
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, State& capture, boost::mpl::true_) const
        {
                return land<Direction>(jumper, capture) | reverse<Direction>(jumper, capture);
        }

        template<typename Direction>
        bool reverse(BitIndex jumper, State& capture) const
        {
                return scan< typename rotate< Direction, angle::D180 >::type >(jumper, capture);
        }

        template<typename Direction>
        bool land(BitIndex jumper, State& capture) const
        {
                // tag dispatching on king jump landing range after intermediate captures
                return land_dispatch<Direction>(jumper, capture, typename Rules::land_range());
        }

        // overload for kings that land immediately if the intermediate capture is a king, and slide through otherwise
        template<typename Direction>
        void land_dispatch(BitIndex jumper, State& capture, rules::range::distance_1K) const
        {
                if (capture.is_king(Board::prev<Direction>(jumper)))
                        land_dispatch<Direction>(jumper, capture, rules::range::distance_1());
                else
                        land_dispatch<Direction>(jumper, capture, rules::range::distance_N());
        }

        // overload for kings that can only land on the immediately adjacent square
        template<typename Direction>
        bool land_dispatch(BitIndex jumper, State& capture, rules::range::distance_1) const
        {
                return (
                        turn<Direction>(jumper, capture) |
                        scan<Direction>(jumper, capture)
                );
        }

        // overload for kings that can land on any square along the current direction
        template<typename Direction>
        bool land_dispatch(BitIndex jumper, State& capture, rules::range::distance_N) const
        {
                BOOST_ASSERT(bit::is_element(jumper, capture.path()));
                auto found_next = false;
                do {
                        found_next |= turn<Direction>(jumper, capture);
                        Board::advance<Direction>(jumper);
                } while (bit::is_element(jumper, capture.path()));
                return found_next |= jump<Direction>(jumper, capture);
        }

        template<typename Direction>
        bool turn(BitIndex jumper, State& capture) const
        {
                // tag dispatching on king turn directions
                return turn_dispatch<Direction>(jumper, capture, typename Rules::king_turn_directions());
        }

        // overload for turns in all the 6 non-parallel diagonal and orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::all) const
        {
                return (
                        turn_dispatch<Direction>(jumper, capture, rules::directions::diag()) |
                        turn_dispatch<Direction>(jumper, capture, rules::directions::orth())
                );
        }

        // overload for turns in the 2 sideways directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::diag) const
        {
                return (
                        scan< typename rotate< Direction, angle::R090 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L090 >::type >(jumper, capture)
                );
        }

        // overload for turns in the remaining 4 diagonal or orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::orth) const
        {
                return (
                        scan< typename rotate< Direction, angle::R045 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L045 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::R135 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L135 >::type >(jumper, capture)
                );
        }

        template<typename Direction>
        bool scan(BitIndex jumper, State& capture) const
        {
                slide<Direction>(jumper, capture.template path<Direction>());
                return jump<Direction>(jumper, capture);
        }

        template<typename Direction>
        void slide(BitIndex& jumper, BitBoard path) const
        {
                // tag dispatching on king range
                slide_dispatch<Direction>(jumper, path, typename Rules::king_range());
        }

        // overload for short ranged kings
        template<typename Direction>
        void slide_dispatch(BitIndex& jumper, BitBoard /* path */, rules::range::distance_1) const
        {
                Board::advance<Direction>(jumper);
        }

        // overload for long ranged kings
        template<typename Direction>
        void slide_dispatch(BitIndex& jumper, BitBoard path, rules::range::distance_N) const
        {
                do Board::advance<Direction>(jumper); while (bit::is_element(jumper, path));
        }

        template<typename Direction>
        bool jump(BitIndex jumper, State& capture) const
        {
                if (!bit::is_element(jumper, capture.template targets<Direction>()))
                        return false;

                capture.make(jumper);
                precedence<Direction>(jumper, capture); // recursively find more jumps
                capture.undo(jumper);
                return true;
        }

        template<typename Direction>
        void add_king_jump(BitIndex dest_sq, State& capture) const
        {
                auto const ambiguous = rules::is_check_jump_uniqueness<Rules>::value && capture.is_ambiguous();

                // tag dispatching on king halt after final capture
                add_king_jump_dispatch<Direction>(dest_sq, capture, ambiguous, typename Rules::halt_range());
        }

        // overload for kings that halt immediately if the final capture is a king, and slide through otherwise
        template<typename Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, State& capture, bool ambiguous, rules::range::distance_1K) const
        {
                if (capture.is_king(Board::prev<Direction>(dest_sq)))
                        add_king_jump_dispatch<Direction>(dest_sq, capture, ambiguous, rules::range::distance_1());
                else
                        add_king_jump_dispatch<Direction>(dest_sq, capture, ambiguous, rules::range::distance_N());
        }

        // overload for kings that halt immediately after the final capture
        template<typename Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, State& capture, bool ambiguous, rules::range::distance_1) const
        {
                add_king_jump(dest_sq, capture, ambiguous);
        }

        // overload for kings that slide through after the final capture
        template<typename Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, State& capture, bool ambiguous, rules::range::distance_N) const
        {
                // NOTE: capture.template path<Direction>() would be an ERROR here
                // because we need all landing squares rather than the directional launching squares subset
                BOOST_ASSERT(bit::is_element(dest_sq, capture.path()));
                do {
                        add_king_jump(dest_sq, capture, ambiguous);
                        Board::advance<Direction>(dest_sq);
                } while (bit::is_element(dest_sq, capture.path()));
        }

        void add_king_jump(BitIndex dest_sq, State& capture, bool ambiguous) const
        {
                // tag dispatching on promotion condition
                add_king_jump_dispatch(dest_sq, capture, typename Rules::pawn_promotion());
                if (ambiguous)
                        capture.remove_non_unique_back();
        }

        // overload for pawns that promote apres-fini
        void add_king_jump_dispatch(BitIndex dest_sq, State& capture, rules::promotion::apres_fini) const
        {
                capture.template add_king_jump<Color>(dest_sq);
        }

        // overload for pawns that promote en-passant
        void add_king_jump_dispatch(BitIndex dest_sq, State& capture, rules::promotion::en_passant) const
        {
                if (!capture.is_promotion())
                        capture.template add_king_jump<Color>(dest_sq);
                else
                        capture.template add_pawn_jump<Color, capture::with::king>(dest_sq);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
