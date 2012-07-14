#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include "Driver_fwd.hpp"
#include "Selection.hpp"
#include "../bit/Bit.hpp"
#include "../board/Compass.hpp"
#include "../board/Degrees.hpp"
#include "../board/Shift.hpp"
#include "../capture/State.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../rules/Enum.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for king jumps
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::king, select::Jumps, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef angle::Compass<Color, Board> Compass;
        typedef capture::State<Rules, Board> State;
        
public:
        template<typename Position>
        static void generate(Position const& p, Stack& moves)
        {
                State capture(p, moves);
                generate(p, capture);
        }

        template<typename Position>
        static void generate(Position const& p, State& capture)
        {
                generate_precede(p, capture);
        }

        template<typename Direction>
        static bool promote_en_passant(BitIndex jumper, State& capture)
        {
                return scan_next<Direction>(jumper, capture);
        }

        template<typename Position>
        static int count(Position const& p)
        {
                Stack moves;
                generate(p, moves);
                return static_cast<int>(moves.size());
        }

        template<typename Position>
        static bool detect(Position const& p)
        {
                return detect(p.kings(Color), p.pieces(!Color), not_occupied(p));
        }

private:
        template<typename Position>
        static void generate_precede(Position const& p, State& capture)
        {
                // tag dispatching on relative king capture precedence
                generate_precede(p, capture, typename Rules::is_relative_king_precedence());
        }

        // overload for no relative king capture precedence
        template<typename Position>
        static void generate_precede(Position const& p, State& capture, boost::mpl::false_)
        {
                serialize(p.kings(Color), capture);
        }

        // overload for relative king capture precedence
        template<typename Position>
        static void generate_precede(Position const& p, State& capture, boost::mpl::true_)
        {
                capture.toggle_with_king();
                serialize(p.kings(Color), capture);
                capture.toggle_with_king();
        }

        static void serialize(BitBoard active_kings, State& capture)
        {
                BitIndex jumper;
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        jumper = bit::get_first(active_kings);
                        capture.launch(jumper);
                        generate(jumper, capture);
                        capture.finish(jumper);
                        bit::clear_first(active_kings);
                } while (active_kings);
        }

        static void generate(BitIndex jumper, State& capture)
        {
                // tag dispatching on king capture directions
                generate_dispatch(jumper, capture, typename Rules::king_jump_directions());
        }

        // overload for kings that capture in the 8 orthogonal and diagonal directions
        static void generate_dispatch(BitIndex jumper, State& capture, rules::directions::all)
        {
                generate_dispatch(jumper, capture, rules::directions::orth());
                generate_dispatch(jumper, capture, rules::directions::diag());
        }

        // overload for kings that capture in the 4 orthogonal directions
        static void generate_dispatch(BitIndex jumper, State& capture, rules::directions::orth)
        {
                generate<typename Compass::left >(jumper, capture);
                generate<typename Compass::right>(jumper, capture);
                generate<typename Compass::up   >(jumper, capture);
                generate<typename Compass::down >(jumper, capture);
        }

        // overload for kings that capture in the 4 diagonal directions
        static void generate_dispatch(BitIndex jumper, State& capture, rules::directions::diag)
        {
                generate<typename Compass::left_up   >(jumper, capture);
                generate<typename Compass::right_up  >(jumper, capture);
                generate<typename Compass::left_down >(jumper, capture);
                generate<typename Compass::right_down>(jumper, capture);
        }

        static bool detect(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied)
        {
                // tag dispatching on king capture directions
                return detect_dispatch(active_kings, passive_pieces, not_occupied, typename Rules::king_jump_directions());
        }

        // overload for kings that capture in the 8 orthogonal and diagonal directions
        static bool detect_dispatch(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::all)
        {
                return (
                        detect_dispatch(active_kings, passive_pieces, not_occupied, rules::directions::orth()) ||
                        detect_dispatch(active_kings, passive_pieces, not_occupied, rules::directions::diag())
                );
        }

        // overload for kings that capture in the 4 orthogonal directions
        static bool detect_dispatch(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::orth)
        {
                return (
                        detect<typename Compass::left >(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Compass::right>(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Compass::up   >(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Compass::down >(active_kings, passive_pieces, not_occupied)
                );
        }

        // overload for kings that capture in the 4 diagonal directions
        static bool detect_dispatch(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, rules::directions::diag)
        {
                return (
                        detect<typename Compass::left_up   >(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Compass::right_up  >(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Compass::left_down >(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Compass::right_down>(active_kings, passive_pieces, not_occupied)
                );
        }

        template<typename Direction>
        static void generate(BitIndex jumper, State& capture)
        {
                slide<Direction>(jumper, capture.template path<Direction>());
                if (bit::is_element(jumper, capture.template targets<Direction>())) {
                        capture.make(jumper);
                        generate_next<Direction>(jumper, capture);
                        capture.undo(jumper);
                }
        }

        template<typename Direction>
        static void generate_next(BitIndex jumper, State& capture)
        {
                Board::advance<Direction>(jumper);
                if (
                        !scan_next<Direction>(jumper, capture) &&
                        capture.is_improvement()
                ) {
                        if (capture.improvement_is_strict())
                                capture.improve();
                        capture.template add_king_jump<Color, Direction>(jumper);
                }
        }

        template<typename Direction>
        static bool scan_next(BitIndex jumper, State& capture)
        {
                // tag dispatching on king capture direction reversal
                return scan_next_dispatch<Direction>(jumper, capture, typename Rules::is_jump_direction_reversal());
        }

        // overload for kings that cannot reverse their capture direction
        template<typename Direction>
        static bool scan_next_dispatch(BitIndex jumper, State& capture, boost::mpl::false_)
        {
                return land<Direction>(jumper, capture);
        }

        // overload for kings that can reverse their capture direction
        template<typename Direction>
        static bool scan_next_dispatch(BitIndex jumper, State& capture, boost::mpl::true_)
        {
                return (
                        reverse<Direction>(jumper, capture) |
                        land<Direction>(jumper, capture)
                );
        }

        template<typename Direction>
        static bool reverse(BitIndex jumper, State& capture)
        {
                return scan< typename rotate< Direction, angle::D180 >::type >(jumper, capture);
        }

        template<typename Direction>
        static bool land(BitIndex jumper, State& capture)
        {
                // tag dispatching on king capture landing range after intermediate captures
                return land_dispatch<Direction>(jumper, capture, typename Rules::land_range());
        }

        // overload for kings that can only land on the immediately adjacent square
        template<typename Direction>
        static bool land_dispatch(BitIndex jumper, State& capture, rules::range::distance_1)
        {
                return (
                        turn<Direction>(jumper, capture) |
                        scan<Direction>(jumper, capture)
                );
        }

        // overload for kings that can land on any square along the current direction
        template<typename Direction>
        static bool land_dispatch(BitIndex jumper, State& capture, rules::range::distance_N)
        {
                BOOST_ASSERT(bit::is_element(jumper, capture.path()));
                bool found_capture = false;
                do {
                        found_capture |= turn<Direction>(jumper, capture);
                        Board::advance<Direction>(jumper);
                } while (bit::is_element(jumper, capture.path()));
                return found_capture |= jump<Direction>(jumper, capture);
        }

        template<typename Direction>
        static bool turn(BitIndex jumper, State& capture)
        {
                // tag dispatching on king turn directions
                return turn_dispatch<Direction>(jumper, capture, typename Rules::king_turn_directions());
        }

        // overload for turns in all the 6 non-parallel orthogonal and diagonal directions
        template<typename Direction>
        static bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::all)
        {
                return (
                        turn_dispatch<Direction>(jumper, capture, rules::directions::orth()) |
                        turn_dispatch<Direction>(jumper, capture, rules::directions::diag())
                );
        }

        // overload for turns in the remaining 4 diagonal or orthogonal directions
        template<typename Direction>
        static bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::orth)
        {
                return (
                        scan< typename rotate< Direction, angle::R045 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L045 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::R135 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L135 >::type >(jumper, capture)
                );
        }

        // overload for turns in the 2 sideways directions
        template<typename Direction>
        static bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::diag)
        {
                return (
                        scan< typename rotate< Direction, angle::R090 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L090 >::type >(jumper, capture)
                );
        }

        template<typename Direction>
        static bool scan(BitIndex jumper, State& capture)
        {
                slide<Direction>(jumper, capture.template path<Direction>());
                return jump<Direction>(jumper, capture);
        }

        template<typename Direction>
        static void slide(BitIndex& jumper, BitBoard path)
        {
                // tag dispatching on king range
                slide_dispatch<Direction>(jumper, path, typename Rules::king_range());
        }

        // overload for short ranged kings
        template<typename Direction>
        static void slide_dispatch(BitIndex& jumper, BitBoard /* path */, rules::range::distance_1)
        {
                Board::advance<Direction>(jumper);
        }

        // overload for long ranged kings
        template<typename Direction>
        static void slide_dispatch(BitIndex& jumper, BitBoard path, rules::range::distance_N)
        {
                do Board::advance<Direction>(jumper); while (bit::is_element(jumper, path));
        }

        template<typename Direction>
        static bool jump(BitIndex jumper, State& capture)
        {
                if (bit::is_element(jumper, capture.template targets<Direction>())) {
                        capture.make(jumper);
                        generate_next<Direction>(jumper, capture);
                        capture.undo(jumper);
                        return true;
                } else
                        return false;
        }

        template<typename Direction>
        static bool detect(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied)
        {
                // partial specialiations of Sandwich for king range
                return !bit::is_zero(
                        Sandwich<Board, Direction, typename Rules::king_range>()
                        (active_kings, passive_pieces, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl