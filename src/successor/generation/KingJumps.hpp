#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
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
struct generator<Color, Material::king, select::Jumps, Position>
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;
        typedef capture::State<Position> State;
        
public:
        void operator()(Position const& p, Stack& moves)
        {
                if (auto const active_kings = p.kings(Color)) {
                        State capture(p, moves);
                        select(active_kings, capture);
                }
        }

        void operator()(Position const& p, State& capture)
        {
                if (auto const active_kings = p.kings(Color))
                        select(active_kings, capture);
        }

        template<typename Direction>
        bool promote_en_passant(BitIndex jumper, State& capture)
        {
                BOOST_ASSERT((is_promotion_sq<Color, Board>(jumper)));
                return find_next<Direction>(jumper, capture);
        }

private:
        void select(BitBoard active_kings, State& capture)
        {
                // tag dispatching on relative king capture precedence
                select_dispatch(active_kings, capture, typename Rules::is_relative_king_precedence());
        }

        // overload for no relative king capture precedence
        void select_dispatch(BitBoard active_kings, State& capture, boost::mpl::false_)
        {
                serialize(active_kings, capture);
        }

        // overload for relative king capture precedence
        void select_dispatch(BitBoard active_kings, State& capture, boost::mpl::true_)
        {
                capture.toggle_with_king();
                serialize(active_kings, capture);
                capture.toggle_with_king();
        }

        void serialize(BitBoard active_kings, State& capture)
        {
                BitIndex jumper;
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        jumper = bit::get_first(active_kings);
                        capture.launch(jumper);
                        branch(jumper, capture);
                        capture.finish(jumper);
                        bit::clear_first(active_kings);
                } while (active_kings);
        }

        void branch(BitIndex jumper, State& capture)
        {
                // tag dispatching on king capture directions
                branch_dispatch(jumper, capture, typename Rules::king_jump_directions());
        }

        // overload for kings that capture in the 8 diagonal and orthogonal directions
        void branch_dispatch(BitIndex jumper, State& capture, rules::directions::all)
        {
                branch_dispatch(jumper, capture, rules::directions::diag());
                branch_dispatch(jumper, capture, rules::directions::orth());
        }

        // overload for kings that capture in the 4 diagonal directions
        void branch_dispatch(BitIndex jumper, State& capture, rules::directions::diag)
        {
                find_first<typename Compass::left_up   >(jumper, capture);
                find_first<typename Compass::right_up  >(jumper, capture);
                find_first<typename Compass::left_down >(jumper, capture);
                find_first<typename Compass::right_down>(jumper, capture);
        }

        // overload for kings that capture in the 4 orthogonal directions
        void branch_dispatch(BitIndex jumper, State& capture, rules::directions::orth)
        {
                find_first<typename Compass::left >(jumper, capture);
                find_first<typename Compass::right>(jumper, capture);
                find_first<typename Compass::up   >(jumper, capture);
                find_first<typename Compass::down >(jumper, capture);
        }

        template<typename Direction>
        void find_first(BitIndex jumper, State& capture)
        {
                slide<Direction>(jumper, capture.template path<Direction>());
                if (bit::is_element(jumper, capture.template targets<Direction>())) {
                        capture.make(jumper);
                        add_jump<Direction>(jumper, capture);   // recursively find more jumps
                        capture.undo(jumper);
                }
        }

        template<typename Direction>
        void add_jump(BitIndex jumper, State& capture)
        {
                Board::advance<Direction>(jumper);
                if (
                        !find_next<Direction>(jumper, capture) &&
                        capture.is_improvement()
                ) {
                        if (capture.improvement_is_strict())
                                capture.improve();
                        capture.template add_king_jump<Color, Direction>(jumper);
                }
        }

        template<typename Direction>
        bool find_next(BitIndex jumper, State& capture)
        {
                // tag dispatching on king capture direction reversal
                return find_next_dispatch<Direction>(jumper, capture, typename Rules::is_jump_direction_reversal());
        }

        // overload for kings that cannot reverse their capture direction
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, State& capture, boost::mpl::false_)
        {
                return land<Direction>(jumper, capture);
        }

        // overload for kings that can reverse their capture direction
        template<typename Direction>
        bool find_next_dispatch(BitIndex jumper, State& capture, boost::mpl::true_)
        {
                return land<Direction>(jumper, capture) | reverse<Direction>(jumper, capture);
        }

        template<typename Direction>
        bool reverse(BitIndex jumper, State& capture)
        {
                return scan< typename rotate< Direction, angle::D180 >::type >(jumper, capture);
        }

        template<typename Direction>
        bool land(BitIndex jumper, State& capture)
        {
                // tag dispatching on king capture landing range after intermediate captures
                return land_dispatch<Direction>(jumper, capture, typename Rules::land_range());
        }

        // overload for kings that can only land on the immediately adjacent square
        template<typename Direction>
        bool land_dispatch(BitIndex jumper, State& capture, rules::range::distance_1)
        {
                return (
                        turn<Direction>(jumper, capture) |
                        scan<Direction>(jumper, capture)
                );
        }

        // overload for kings that can land on any square along the current direction
        template<typename Direction>
        bool land_dispatch(BitIndex jumper, State& capture, rules::range::distance_N)
        {
                BOOST_ASSERT(bit::is_element(jumper, capture.path()));
                bool found_next = false;
                do {
                        found_next |= turn<Direction>(jumper, capture);
                        Board::advance<Direction>(jumper);
                } while (bit::is_element(jumper, capture.path()));
                return found_next |= jump<Direction>(jumper, capture);
        }

        template<typename Direction>
        bool turn(BitIndex jumper, State& capture)
        {
                // tag dispatching on king turn directions
                return turn_dispatch<Direction>(jumper, capture, typename Rules::king_turn_directions());
        }

        // overload for turns in all the 6 non-parallel diagonal and orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::all)
        {
                return (
                        turn_dispatch<Direction>(jumper, capture, rules::directions::diag()) |
                        turn_dispatch<Direction>(jumper, capture, rules::directions::orth())
                );
        }

        // overload for turns in the 2 sideways directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::diag)
        {
                return (
                        scan< typename rotate< Direction, angle::R090 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L090 >::type >(jumper, capture)
                );
        }

        // overload for turns in the remaining 4 diagonal or orthogonal directions
        template<typename Direction>
        bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::orth)
        {
                return (
                        scan< typename rotate< Direction, angle::R045 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L045 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::R135 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L135 >::type >(jumper, capture)
                );
        }

        template<typename Direction>
        bool scan(BitIndex jumper, State& capture)
        {
                slide<Direction>(jumper, capture.template path<Direction>());
                return jump<Direction>(jumper, capture);
        }

        template<typename Direction>
        void slide(BitIndex& jumper, BitBoard path)
        {
                // tag dispatching on king range
                slide_dispatch<Direction>(jumper, path, typename Rules::king_range());
        }

        // overload for short ranged kings
        template<typename Direction>
        void slide_dispatch(BitIndex& jumper, BitBoard /* path */, rules::range::distance_1)
        {
                Board::advance<Direction>(jumper);
        }

        // overload for long ranged kings
        template<typename Direction>
        void slide_dispatch(BitIndex& jumper, BitBoard path, rules::range::distance_N)
        {
                do Board::advance<Direction>(jumper); while (bit::is_element(jumper, path));
        }

        template<typename Direction>
        bool jump(BitIndex jumper, State& capture)
        {
                if (!bit::is_element(jumper, capture.template targets<Direction>())) 
                        return false;                   // terminated

                capture.make(jumper);
                add_jump<Direction>(jumper, capture);   // recursively find more jumps
                capture.undo(jumper);                
                return true;                            // not terminated 
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
