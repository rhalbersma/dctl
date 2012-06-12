#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include "Driver_fwd.hpp"
#include "Selection.hpp"
#include "../bit/Bit.hpp"
#include "../board/Degrees.hpp"
#include "../board/Direction.hpp"
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

        typedef angle::Direction<Color, Board> Direction;
        typedef capture::State<Rules, Board> State;
        
public:
        template<template<typename, typename> class Position>
        static void generate(Position<Rules, Board> const& p, Stack& moves)
        {
                State capture(p, moves);
                generate(p, capture);
        }

        template<template<typename, typename> class Position>
        static void generate(Position<Rules, Board> const& p, State& capture)
        {
                generate_precede(p, capture);
        }

        template<typename Index>
        static bool promote_en_passant(BitBoard jumper, State& capture)
        {
                return scan_next<Index>(jumper, capture);
        }

        template<template<typename, typename> class Position>
        static int count(Position<Rules, Board> const& p)
        {
                Stack moves;
                generate(p, moves);
                return static_cast<int>(moves.size());
        }

        template<template<typename, typename> class Position>
        static bool detect(Position<Rules, Board> const& p)
        {
                return detect(p.kings(Color), p.pieces(!Color), not_occupied(p));
        }

private:
        template<template<typename, typename> class Position>
        static void generate_precede(Position<Rules, Board> const& p, State& capture)
        {
                // tag dispatching on relative king capture precedence
                generate_precede(
                        p, capture,
                        typename Rules::is_relative_king_precedence()
                );
        }

        // partial specialization for relative king capture precedence
        template<template<typename, typename> class Position>
        static void generate_precede(
                Position<Rules, Board> const& p, State& capture, 
                boost::mpl::true_
        )
        {
                capture.toggle_with_king();
                generate_precede(
                        p, capture, 
                        boost::mpl::false_()
                );
                capture.toggle_with_king();
        }

        // partial specialization for no relative king capture precedence
        template<template<typename, typename> class Position>
        static void generate_precede(
                Position<Rules, Board> const& p, State& capture, 
                boost::mpl::false_
        )
        {
                serialize(p.kings(Color), capture);
        }

        static void serialize(BitBoard active_kings, State& capture)
        {
                BitBoard jumper;
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        jumper = bit::get_first(active_kings);
                        capture.launch(jumper);
                        generate(jumper, capture);
                        capture.finish(jumper);
                        bit::clear_first(active_kings);
                } while (active_kings);
        }

        static void generate(BitBoard jumper, State& capture)
        {
                // tag dispatching on king capture directions
                generate_dispatch(
                        jumper, capture,
                        typename Rules::king_jump_directions()
                );
        }

        // partial specialization for kings that capture in the 8 orthogonal and diagonal directions
        static void generate_dispatch(
                BitBoard jumper, State& capture, 
                rules::directions::all
        )
        {
                generate_dispatch(jumper, capture, rules::directions::orth());
                generate_dispatch(jumper, capture, rules::directions::diag());
        }

        // partial specialization for kings that capture in the 4 orthogonal directions
        static void generate_dispatch(
                BitBoard jumper, State& capture, 
                rules::directions::orth
        )
        {
                generate<typename Direction::left >(jumper, capture);
                generate<typename Direction::right>(jumper, capture);
                generate<typename Direction::up   >(jumper, capture);
                generate<typename Direction::down >(jumper, capture);
        }

        // partial specialization for kings that capture in the 4 diagonal directions
        static void generate_dispatch(
                BitBoard jumper, State& capture, 
                rules::directions::diag
        )
        {
                generate<typename Direction::left_up   >(jumper, capture);
                generate<typename Direction::right_up  >(jumper, capture);
                generate<typename Direction::left_down >(jumper, capture);
                generate<typename Direction::right_down>(jumper, capture);
        }

        static bool detect(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied)
        {
                // tag dispatching on king capture directions
                return detect_dispatch(
                        active_kings, passive_pieces, not_occupied,
                        typename Rules::king_jump_directions()
                );
        }

        // partial specialization for kings that capture in the 8 orthogonal and diagonal directions
        static bool detect_dispatch(
                BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, 
                rules::directions::all
        )
        {
                return (
                        detect_dispatch(active_kings, passive_pieces, not_occupied, rules::directions::orth()) ||
                        detect_dispatch(active_kings, passive_pieces, not_occupied, rules::directions::diag())
                );
        }

        // partial specialization for kings that capture in the 4 orthogonal directions
        static bool detect_dispatch(
                BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, 
                rules::directions::orth
        )
        {
                return (
                        detect<typename Direction::left >(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Direction::right>(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Direction::up   >(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Direction::down >(active_kings, passive_pieces, not_occupied)
                );
        }

        // partial specialization for kings that capture in the 4 diagonal directions
        static bool detect_dispatch(
                BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, 
                rules::directions::diag
        )
        {
                return (
                        detect<typename Direction::left_up   >(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Direction::right_up  >(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Direction::left_down >(active_kings, passive_pieces, not_occupied) ||
                        detect<typename Direction::right_down>(active_kings, passive_pieces, not_occupied)
                );
        }

        template<typename Index>
        static void generate(BitBoard jumper, State& capture)
        {
                slide<Index>(jumper, capture.template path<Index>());
                if (jumper & capture.template targets<Index>()) {
                        capture.make(jumper);
                        generate_next<Index>(jumper, capture);
                        capture.undo(jumper);
                }
        }

        template<typename Index>
        static void generate_next(BitBoard jumper, State& capture)
        {
                Board::advance<Index>()(jumper);
                if (
                        !scan_next<Index>(jumper, capture) &&
                        capture.is_improvement()
                ) {
                        if (capture.improvement_is_strict())
                                capture.improve();
                        capture.template add_king_jump<Color, Index>(jumper);
                }
        }

        template<typename Index>
        static bool scan_next(BitBoard jumper, State& capture)
        {
                // tag dispatching on king capture direction reversal
                return scan_next_dispatch<Index>(
                        jumper, capture,
                        typename Rules::is_jump_direction_reversal()
                );
        }

        // partial specialization for kings that cannot reverse their capture direction
        template<typename Index>
        static bool scan_next_dispatch(
                BitBoard jumper, State& capture, 
                boost::mpl::false_
        )
        {
                return land<Index>(jumper, capture);
        }

        // partial specialization for kings that can reverse their capture direction
        template<typename Index>
        static bool scan_next_dispatch(
                BitBoard jumper, State& capture, 
                boost::mpl::true_
        )
        {
                return (
                        reverse<Index>(jumper, capture) |
                        scan_next_dispatch<Index>(
                                jumper, capture, 
                                boost::mpl::false_()
                        )
                );
        }

        template<typename Index>
        static bool reverse(BitBoard jumper, State& capture)
        {
                return scan< typename rotate< Index, angle::D180 >::type >(jumper, capture);
        }

        template<typename Index>
        static bool land(BitBoard jumper, State& capture)
        {
                // tag dispatching on king capture landing range after intermediate captures
                return land_dispatch<Index>(
                        jumper, capture,
                        typename Rules::land_range()
                );
        }

        // partial specialization for kings that can only land on the immediately adjacent square
        template<typename Index>
        static bool land_dispatch(
                BitBoard jumper, State& capture, 
                rules::range::distance_1
        )
        {
                return (
                        turn<Index>(jumper, capture) |
                        scan<Index>(jumper, capture)
                );
        }

        // partial specialization for kings that can land on any square along the current direction
        template<typename Index>
        static bool land_dispatch(
                BitBoard jumper, State& capture, 
                rules::range::distance_N
        )
        {
                BOOST_ASSERT(jumper & capture.path());
                bool found_capture = false;
                do {
                        found_capture |= turn<Index>(jumper, capture);
                        Board::advance<Index>()(jumper);
                } while (jumper & capture.path());
                return found_capture |= jump<Index>(jumper, capture);
        }

        template<typename Index>
        static bool turn(BitBoard jumper, State& capture)
        {
                // tag dispatching on king turn directions
                return turn_dispatch<Index>(
                        jumper, capture,
                        typename Rules::king_turn_directions()
                );
        }

        // partial specialization for turns in all the 6 non-parallel orthogonal and diagonal directions
        template<typename Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, 
                rules::directions::all
        )
        {
                return (
                        turn_dispatch<Index>(jumper, capture, rules::directions::orth()) |
                        turn_dispatch<Index>(jumper, capture, rules::directions::diag())
                );
        }

        // partial specialization for turns in the remaining 4 diagonal or orthogonal directions
        template<typename Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, 
                rules::directions::orth
        )
        {
                return (
                        scan< typename rotate< Index, angle::R045 >::type >(jumper, capture) |
                        scan< typename rotate< Index, angle::L045 >::type >(jumper, capture) |
                        scan< typename rotate< Index, angle::R135 >::type >(jumper, capture) |
                        scan< typename rotate< Index, angle::L135 >::type >(jumper, capture)
                );
        }

        // partial specialization for turns in the 2 sideways directions
        template<typename Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, 
                rules::directions::diag
        )
        {
                return (
                        scan< typename rotate< Index, angle::R090 >::type >(jumper, capture) |
                        scan< typename rotate< Index, angle::L090 >::type >(jumper, capture)
                );
        }

        template<typename Index>
        static bool scan(BitBoard jumper, State& capture)
        {
                slide<Index>(jumper, capture.template path<Index>());
                return jump<Index>(jumper, capture);
        }

        template<typename Index>
        static void slide(BitBoard& jumper, BitBoard path)
        {
                // tag dispatching on king range
                slide_dispatch<Index>(
                        jumper, path,
                        typename Rules::king_range()
                );
        }

        // partial specialization for short ranged kings
        template<typename Index>
        static void slide_dispatch(
                BitBoard& jumper, BitBoard /* path */, 
                rules::range::distance_1
        )
        {
                Board::advance<Index>()(jumper);
        }

        // partial specialization for long ranged kings
        template<typename Index>
        static void slide_dispatch(
                BitBoard& jumper, BitBoard path, 
                rules::range::distance_N
        )
        {
                do Board::advance<Index>()(jumper); while (jumper & path);
        }

        template<typename Index>
        static bool jump(BitBoard jumper, State& capture)
        {
                if (jumper & capture.template targets<Index>()) {
                        capture.make(jumper);
                        generate_next<Index>(jumper, capture);
                        capture.undo(jumper);
                        return true;
                } else
                        return false;
        }

        template<typename Index>
        static bool detect(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied)
        {
                // partial specialiations of Sandwich for king range
                return !bit::is_zero(
                        Sandwich<Board, Index, typename Rules::king_range>()
                        (active_kings, passive_pieces, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
