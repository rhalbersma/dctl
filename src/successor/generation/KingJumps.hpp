#pragma once
#include <functional>                   // function
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/mpl/bool.hpp>           // false_, true_
#include <boost/utility.hpp>            // noncopyable
#include "Generator_fwd.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Degrees.hpp"
#include "../../board/Shift.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps generation
template<bool Color, typename Position>
struct generator<Color, Material::king, Jumps, Position>
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&)>
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;
        typedef capture::State<Position> State;

        // representation

        State& capture_;

public:
        // structors
        
        explicit generator(State& c)
        : 
                capture_(c) 
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                if (auto const active_kings = p.kings(Color))
                        select(active_kings);
        }

        template<typename Direction>
        bool promote_en_passant(BitIndex jumper) const
        {
                BOOST_ASSERT((is_promotion_sq<Color, Board>(jumper)));
                return find_next<Direction>(jumper);
        }

private:
        void select(BitBoard active_kings) const
        {
                // tag dispatching on relative king jump precedence
                select_dispatch(active_kings, typename Rules::is_relative_king_precedence());
        }

        // overload for no relative king jump precedence
        void select_dispatch(BitBoard active_kings, boost::mpl::false_) const
        {
                serialize(active_kings);
        }

        // overload for relative king jump precedence
        void select_dispatch(BitBoard active_kings, boost::mpl::true_) const
        {
                capture_.toggle_with_king();
                serialize(active_kings);
                capture_.toggle_with_king();
        }

        void serialize(BitBoard active_kings) const
        {
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        find(bit::get_first(active_kings));
                        bit::clear_first(active_kings);
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
                branch_dispatch(jumper, typename Rules::king_jump_directions());
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
                        precedence<Direction>(jumper); // recursively find more jumps
                        capture_.undo(jumper);
                }
        }

        template<typename Direction>
        void precedence(BitIndex jumper) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch<Direction>(jumper, typename Rules::is_precedence());
        }

        // overload for no majority precedence
        template<typename Direction>
        void precedence_dispatch(BitIndex jumper, boost::mpl::false_) const
        {
                Advance<Board, Direction>()(jumper);
                if (!find_next<Direction>(jumper))
                        add_king_jump<Direction>(jumper);
        }

        // overload for majority precedence
        template<typename Direction>
        void precedence_dispatch(BitIndex jumper, boost::mpl::true_) const
        {
                Advance<Board, Direction>()(jumper);
                if (
                        !find_next<Direction>(jumper) &&
                        capture_.greater_equal()
                ) {
                        if (capture_.not_equal_to())
                                capture_.improve();
                        add_king_jump<Direction>(jumper);
                }
        }

        template<typename Direction>
        bool find_next(BitIndex jumper) const
        {
                // tag dispatching on king jump direction reversal
                return find_next_dispatch<Direction>(jumper, typename Rules::is_jump_direction_reversal());
        }

        // overload for kings that cannot reverse their capture direction
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, boost::mpl::false_) const
        {
                return land<Direction>(jumper);
        }

        // overload for kings that can reverse their capture direction
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, boost::mpl::true_) const
        {
                return land<Direction>(jumper) | reverse<Direction>(jumper);
        }

        template<typename Direction>
        bool reverse(BitIndex jumper) const
        {
                return scan< typename rotate< Direction, angle::D180 >::type >(jumper);
        }

        template<typename Direction>
        bool land(BitIndex jumper) const
        {
                // tag dispatching on king jump landing range after intermediate captures
                return land_dispatch<Direction>(jumper, typename Rules::land_range());
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
                return (
                        turn<Direction>(jumper) |
                        scan<Direction>(jumper)
                );
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
                        Advance<Board, Direction>()(jumper);
                } while (bit::is_element(jumper, capture_.path()));
                return found_next |= jump<Direction>(jumper);
        }

        template<typename Direction>
        bool turn(BitIndex jumper) const
        {
                // tag dispatching on king turn directions
                return turn_dispatch<Direction>(jumper, typename Rules::king_turn_directions());
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
                        scan< typename rotate< Direction, angle::R090 >::type >(jumper) |
                        scan< typename rotate< Direction, angle::L090 >::type >(jumper)
                );
        }

        // overload for turns in the remaining 4 diagonal or orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, rules::directions::orth) const
        {
                return (
                        scan< typename rotate< Direction, angle::R045 >::type >(jumper) |
                        scan< typename rotate< Direction, angle::L045 >::type >(jumper) |
                        scan< typename rotate< Direction, angle::R135 >::type >(jumper) |
                        scan< typename rotate< Direction, angle::L135 >::type >(jumper)
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
                slide_dispatch<Direction>(jumper, path, typename Rules::king_range());
        }

        // overload for short ranged kings
        template<typename Direction>
        void slide_dispatch(BitIndex& jumper, BitBoard /* path */, rules::range::distance_1) const
        {
                Advance<Board, Direction>()(jumper);
        }

        // overload for long ranged kings
        template<typename Direction>
        void slide_dispatch(BitIndex& jumper, BitBoard path, rules::range::distance_N) const
        {
                do Advance<Board, Direction>()(jumper); while (bit::is_element(jumper, path));
        }

        template<typename Direction>
        bool jump(BitIndex jumper) const
        {
                if (!bit::is_element(jumper, capture_.template targets_with_king<Direction>()))
                        return false;

                capture_.make(jumper);
                precedence<Direction>(jumper); // recursively find more jumps
                capture_.undo(jumper);
                return true;
        }

        template<typename Direction>
        void add_king_jump(BitIndex dest_sq) const
        {
                auto const ambiguous = rules::is_check_jump_uniqueness<Rules>::value && capture_.is_ambiguous();

                // tag dispatching on king halt after final capture
                add_king_jump_dispatch<Direction>(dest_sq, ambiguous, typename Rules::halt_range());
        }

        // overload for kings that halt immediately if the final capture is a king, and slide through otherwise
        template<typename Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, bool ambiguous, rules::range::distance_1K) const
        {
                if (capture_.is_king(Prev<Board, Direction>()(dest_sq)))
                        add_king_jump_dispatch<Direction>(dest_sq, ambiguous, rules::range::distance_1());
                else
                        add_king_jump_dispatch<Direction>(dest_sq, ambiguous, rules::range::distance_N());
        }

        // overload for kings that halt immediately after the final capture
        template<typename Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, bool ambiguous, rules::range::distance_1) const
        {
                add_king_jump(dest_sq, ambiguous);
        }

        // overload for kings that slide through after the final capture
        template<typename Direction>
        void add_king_jump_dispatch(BitIndex dest_sq, bool ambiguous, rules::range::distance_N) const
        {
                // NOTE: capture_.template path<Direction>() would be an ERROR here
                // because we need all halting squares rather than the directional launching squares subset
                BOOST_ASSERT(bit::is_element(dest_sq, capture_.path()));
                do {
                        add_king_jump(dest_sq, ambiguous);
                        Advance<Board, Direction>()(dest_sq);
                } while (bit::is_element(dest_sq, capture_.path()));
        }

        void add_king_jump(BitIndex dest_sq, bool ambiguous) const
        {
                // tag dispatching on promotion condition
                add_king_jump_dispatch(dest_sq, typename Rules::pawn_promotion());
                if (ambiguous)
                        capture_.remove_non_unique_back();
        }

        // overload for pawns that promote apres-fini
        void add_king_jump_dispatch(BitIndex dest_sq, rules::promotion::apres_fini) const
        {
                capture_.template add_king_jump<Color>(dest_sq);
        }

        // overload for pawns that promote en-passant
        void add_king_jump_dispatch(BitIndex dest_sq, rules::promotion::en_passant) const
        {
                if (!capture_.is_promotion())
                        capture_.template add_king_jump<Color>(dest_sq);
                else
                        capture_.template add_pawn_jump<Color, capture::with::king>(dest_sq);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
