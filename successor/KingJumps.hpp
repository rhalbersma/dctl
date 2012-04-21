#pragma once
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include "Driver_fwd.hpp"
#include "Selection.hpp"
#include "../bit/Bit.hpp"
#include "../board/Angle.hpp"
#include "../board/Direction.hpp"
#include "../board/Shift.hpp"
#include "../capture/State.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
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

        typedef board::Direction<Color, Board> Direction;
        typedef capture::State<Rules, Board> State;

public:
        template<template<typename, typename> class Position>
        static void generate(const Position<Rules, Board>& p, Stack& moves)
        {
                State capture(p, moves);
                generate(p, capture);
        }

        template<template<typename, typename> class Position>
        static void generate(const Position<Rules, Board>& p, State& capture)
        {
                generate_precede(p, capture);
        }

        template<int Index>
        static bool promote_en_passant(BitBoard jumper, State& capture)
        {
                return scan_next<Index>(jumper, capture);
        }

        template<template<typename, typename> class Position>
        static int count(const Position<Rules, Board>& p)
        {
                Stack moves;
                generate(p, moves);
                return static_cast<int>(moves.size());
        }

        template<template<typename, typename> class Position>
        static bool detect(const Position<Rules, Board>& p)
        {
                return detect(p.kings(Color), p.pieces(!Color), not_occupied(p));
        }

private:
        template<template<typename, typename> class Position>
        static void generate_precede(const Position<Rules, Board>& p, State& capture)
        {
                // tag dispatching on relative king capture precedence
                generate_precede(
                        p, capture,
                        Int2Type<rules::is_relative_king_precedence<Rules>::value>()
                );
        }

        // partial specialization for relative king capture precedence
        template<template<typename, typename> class Position>
        static void generate_precede(
                const Position<Rules, Board>& p, State& capture, 
                Int2Type<true>
        )
        {
                capture.toggle_with_king();
                generate_precede(
                        p, capture, 
                        Int2Type<false>()
                );
                capture.toggle_with_king();
        }

        // partial specialization for no relative king capture precedence
        template<template<typename, typename> class Position>
        static void generate_precede(
                const Position<Rules, Board>& p, State& capture, 
                Int2Type<false>
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
                        Int2Type<rules::king_jump_directions<Rules>::value>()
                );
        }

        // partial specialization for kings that capture in the 8 orthogonal and diagonal directions
        static void generate_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<rules::dirs_all>
        )
        {
                generate_dispatch(jumper, capture, Int2Type<rules::dirs_orth>());
                generate_dispatch(jumper, capture, Int2Type<rules::dirs_diag>());
        }

        // partial specialization for kings that capture in the 4 orthogonal directions
        static void generate_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<rules::dirs_orth>
        )
        {
                generate<Direction::left >(jumper, capture);
                generate<Direction::right>(jumper, capture);
                generate<Direction::up   >(jumper, capture);
                generate<Direction::down >(jumper, capture);
        }

        // partial specialization for kings that capture in the 4 diagonal directions
        static void generate_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<rules::dirs_diag>
        )
        {
                generate<Direction::left_up   >(jumper, capture);
                generate<Direction::right_up  >(jumper, capture);
                generate<Direction::left_down >(jumper, capture);
                generate<Direction::right_down>(jumper, capture);
        }

        static bool detect(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied)
        {
                // tag dispatching on king capture directions
                return detect_dispatch(
                        active_kings, passive_pieces, not_occupied,
                        Int2Type<rules::king_jump_directions<Rules>::value>()
                );
        }

        // partial specialization for kings that capture in the 8 orthogonal and diagonal directions
        static bool detect_dispatch(
                BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, 
                Int2Type<rules::dirs_all>
        )
        {
                return (
                        detect_dispatch(active_kings, passive_pieces, not_occupied, Int2Type<rules::dirs_orth>()) ||
                        detect_dispatch(active_kings, passive_pieces, not_occupied, Int2Type<rules::dirs_diag>())
                );
        }

        // partial specialization for kings that capture in the 4 orthogonal directions
        static bool detect_dispatch(
                BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, 
                Int2Type<rules::dirs_orth>
        )
        {
                return (
                        detect<Direction::left >(active_kings, passive_pieces, not_occupied) ||
                        detect<Direction::right>(active_kings, passive_pieces, not_occupied) ||
                        detect<Direction::up   >(active_kings, passive_pieces, not_occupied) ||
                        detect<Direction::down >(active_kings, passive_pieces, not_occupied)
                );
        }

        // partial specialization for kings that capture in the 4 diagonal directions
        static bool detect_dispatch(
                BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied, 
                Int2Type<rules::dirs_diag>
        )
        {
                return (
                        detect<Direction::left_up   >(active_kings, passive_pieces, not_occupied) ||
                        detect<Direction::right_up  >(active_kings, passive_pieces, not_occupied) ||
                        detect<Direction::left_down >(active_kings, passive_pieces, not_occupied) ||
                        detect<Direction::right_down>(active_kings, passive_pieces, not_occupied)
                );
        }

        template<int Index>
        static void generate(BitBoard jumper, State& capture)
        {
                slide<Index>(jumper, capture.template path<Index>());
                if (jumper & capture.template targets<Index>()) {
                        capture.make(jumper);
                        generate_next<Index>(jumper, capture);
                        capture.undo(jumper);
                }
        }

        template<int Index>
        static void generate_next(BitBoard jumper, State& capture)
        {
                PushAssign<Board, Index>()(jumper);
                if (
                        !scan_next<Index>(jumper, capture) &&
                        capture.is_improvement()
                ) {
                        if (capture.improvement_is_strict())
                                capture.improve();
                        capture.template add_king_jump<Color, Index>(jumper);
                }
        }

        template<int Index>
        static bool scan_next(BitBoard jumper, State& capture)
        {
                // tag dispatching on king capture direction reversal
                return scan_next_dispatch<Index>(
                        jumper, capture,
                        Int2Type<rules::is_jump_direction_reversal<Rules>::value>()
                );
        }

        // partial specialization for kings that cannot reverse their capture direction
        template<int Index>
        static bool scan_next_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<false>
        )
        {
                return land<Index>(jumper, capture);
        }

        // partial specialization for kings that can reverse their capture direction
        template<int Index>
        static bool scan_next_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<true>
        )
        {
                return (
                        reverse<Index>(jumper, capture) |
                        scan_next_dispatch<Index>(
                                jumper, capture, 
                                Int2Type<false>()
                        )
                );
        }

        template<int Index>
        static bool reverse(BitBoard jumper, State& capture)
        {
                return scan< rotate< angle<Index>, angle<degrees::D180> >::value >(jumper, capture);
        }

        template<int Index>
        static bool land(BitBoard jumper, State& capture)
        {
                // tag dispatching on king capture landing range after intermediate captures
                return land_dispatch<Index>(
                        jumper, capture,
                        Int2Type<rules::king_jump_land<Rules>::value>()
                );
        }

        // partial specialization for kings that can only land on the immediately adjacent square
        template<int Index>
        static bool land_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<rules::land_1>
        )
        {
                return (
                        turn<Index>(jumper, capture) |
                        scan<Index>(jumper, capture)
                );
        }

        // partial specialization for kings that can land on any square along the current direction
        template<int Index>
        static bool land_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<rules::land_N>
        )
        {
                BOOST_ASSERT(jumper & capture.path());
                bool found_capture = false;
                do {
                        found_capture |= turn<Index>(jumper, capture);
                        PushAssign<Board, Index>()(jumper);
                } while (jumper & capture.path());
                return found_capture |= jump<Index>(jumper, capture);
        }

        template<int Index>
        static bool turn(BitBoard jumper, State& capture)
        {
                // tag dispatching on king turn directions
                return turn_dispatch<Index>(
                        jumper, capture,
                        Int2Type<rules::king_turn_directions<Rules>::value>()
                );
        }

        // partial specialization for turns in all the 6 non-parallel orthogonal and diagonal directions
        template<int Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<rules::turn_all>
        )
        {
                return (
                        turn_dispatch<Index>(jumper, capture, Int2Type<rules::turn_orth>()) |
                        turn_dispatch<Index>(jumper, capture, Int2Type<rules::turn_diag>())
                );
        }

        // partial specialization for turns in the remaining 4 diagonal or orthogonal directions
        template<int Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<rules::turn_orth>
        )
        {
                return (
                        scan< rotate< angle<Index>, angle<degrees::R045> >::value >(jumper, capture) |
                        scan< rotate< angle<Index>, angle<degrees::L045> >::value >(jumper, capture) |
                        scan< rotate< angle<Index>, angle<degrees::R135> >::value >(jumper, capture) |
                        scan< rotate< angle<Index>, angle<degrees::L135> >::value >(jumper, capture)
                );
        }

        // partial specialization for turns in the 2 sideways directions
        template<int Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<rules::turn_diag>
        )
        {
                return (
                        scan< rotate< angle<Index>, angle<degrees::R090> >::value >(jumper, capture) |
                        scan< rotate< angle<Index>, angle<degrees::L090> >::value >(jumper, capture)
                );
        }

        template<int Index>
        static bool scan(BitBoard jumper, State& capture)
        {
                slide<Index>(jumper, capture.template path<Index>());
                return jump<Index>(jumper, capture);
        }

        template<int Index>
        static void slide(BitBoard& jumper, BitBoard path)
        {
                // tag dispatching on king range
                slide_dispatch<Index>(
                        jumper, path,
                        Int2Type<rules::king_scan_range<Rules>::value>()
                );
        }

        // partial specialization for short ranged kings
        template<int Index>
        static void slide_dispatch(
                BitBoard& jumper, BitBoard /* path */, 
                Int2Type<rules::scan_1>
        )
        {
                PushAssign<Board, Index>()(jumper);
        }

        // partial specialization for long ranged kings
        template<int Index>
        static void slide_dispatch(
                BitBoard& jumper, BitBoard path, 
                Int2Type<rules::scan_N>
        )
        {
                do PushAssign<Board, Index>()(jumper); while (jumper & path);
        }

        template<int Index>
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

        template<int Index>
        static bool detect(BitBoard active_kings, BitBoard passive_pieces, BitBoard not_occupied)
        {
                // partial specialiations of Sandwich for king range
                return !bit::is_zero(
                        Sandwich<Board, Index, rules::king_scan_range<Rules>::value>()
                        (active_kings, passive_pieces, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
