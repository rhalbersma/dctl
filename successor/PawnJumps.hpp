#pragma once
#include "Driver_fwd.hpp"
#include "KingJumps.hpp"
#include "Selection.hpp"
#include "../bit/Bit.hpp"
#include "../board/Angle.hpp"
#include "../board/Direction.hpp"
#include "../board/Shift.hpp"
#include "../capture/State.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../node/Targets.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for pawn jumps
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::pawn, select::Jumps, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material::king, select::Jumps, Rules, Board> KingJumps;
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
                select_targets(p, capture);
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
                return detect(p.pawns(Color), targets<Color>(p), not_occupied(p));
        }

private:
        template<template<typename, typename> class Position>
        static void select_targets(const Position<Rules, Board>& p, State& capture)
        {
                // tag dispatching on whether pawns can capture kings
                select_targets_dispatch(
                        p, capture,
                        Int2Type<rules::is_pawns_jump_kings<Rules>::value>()
                );
        }

        // partial specialization for pawns that cannot capture kings
        template<template<typename, typename> class Position>
        static void select_targets_dispatch(
                const Position<Rules, Board>& p, State& capture, 
                Int2Type<false>
        )
        {
                capture.toggle_king_targets();
                select_targets_dispatch(
                        p, capture, 
                        Int2Type<true>()
                );
                capture.toggle_king_targets();
        }

        // partial specialization for pawns that can capture kings
        template<template<typename, typename> class Position>
        static void select_targets_dispatch(
                const Position<Rules, Board>& p, State& capture, 
                Int2Type<true>
        )
        {
                generate(p.pawns(Color), capture);
        }

        static void generate(BitBoard active_pawns, State& capture)
        {
                // tag dispatching on man capture directions
                generate_dispatch(
                        active_pawns, capture,
                        Int2Type<rules::pawn_jump_directions<Rules>::value>()
                );
        }

        // partial specialization for pawns that capture in the 8 orthogonal and diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                Int2Type<rules::dirs_all>
        )
        {
                generate_dispatch(active_pawns, capture, Int2Type<rules::dirs_orth>());
                generate_dispatch(active_pawns, capture, Int2Type<rules::dirs_diag>());
        }

        // partial specialization for pawns that capture in the 4 orthogonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                Int2Type<rules::dirs_orth>
        )
        {
                generate<Direction::left >(active_pawns, capture);
                generate<Direction::right>(active_pawns, capture);
                generate<Direction::up   >(active_pawns, capture);
                generate<Direction::down >(active_pawns, capture);
        }

        // partial specialization for pawns that capture in the 4 diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                Int2Type<rules::dirs_diag>
        )
        {
                generate_dispatch(active_pawns, capture, Int2Type<rules::dirs_up  >());
                generate_dispatch(active_pawns, capture, Int2Type<rules::dirs_down>());
        }

        // partial specialization for pawns that capture in the 2 forward diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                Int2Type<rules::dirs_up>
        )
        {
                generate<Direction::left_up >(active_pawns, capture);
                generate<Direction::right_up>(active_pawns, capture);
        }

        // partial specialization for pawns that capture in the 2 backward diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                Int2Type<rules::dirs_down>
        )
        {
                generate<Direction::left_down >(active_pawns, capture);
                generate<Direction::right_down>(active_pawns, capture);
        }

        static bool detect(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied
        )
        {
                // tag dispatching on man capture directions
                return detect_dispatch(
                        active_pawns, passive_pieces, not_occupied,
                        Int2Type<rules::pawn_jump_directions<Rules>::value>()
                );
        }

        // partial specialization for pawns that capture in the 8 orthogonal and diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                Int2Type<rules::dirs_all>
        )
        {
                return (
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, Int2Type<rules::dirs_orth>()) ||
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, Int2Type<rules::dirs_diag>())
                );
        }

        // partial specialization for pawns that capture in the 4 orthogonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                Int2Type<rules::dirs_orth>
        )
        {
                return (
                        detect<Direction::left >(active_pawns, passive_pieces, not_occupied) ||
                        detect<Direction::right>(active_pawns, passive_pieces, not_occupied) ||
                        detect<Direction::up   >(active_pawns, passive_pieces, not_occupied) ||
                        detect<Direction::down >(active_pawns, passive_pieces, not_occupied)
                );
        }

        // partial specialization for pawns that capture in the 4 diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                Int2Type<rules::dirs_diag>
        )
        {
                return (
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, Int2Type<rules::dirs_up  >()) ||
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, Int2Type<rules::dirs_down>())
                );
        }

        // partial specialization for pawns that capture in the 2 forward diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                Int2Type<rules::dirs_up>
        )
        {
                return (
                        detect<Direction::left_up >(active_pawns, passive_pieces, not_occupied) ||
                        detect<Direction::right_up>(active_pawns, passive_pieces, not_occupied)
                );
        }

        // partial specialization for pawns that capture in the 2 backward diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                Int2Type<rules::dirs_down>
        )
        {
                return (
                        detect<Direction::left_down >(active_pawns, passive_pieces, not_occupied) ||
                        detect<Direction::right_down>(active_pawns, passive_pieces, not_occupied)
                );
        }

        template<int Index>
        static void generate(BitBoard active_pawns, State& capture)
        {
                BitBoard jumper, target;
                for (
                        active_pawns &= Pull<Board, Index>()(capture.template targets<Index>());
                        active_pawns;
                        bit::clear_first(active_pawns)
                ) {
                        jumper = bit::get_first(active_pawns);
                        capture.launch(jumper);
                        target = Push<Board, Index>()(jumper);
                        capture.make(target);
                        generate_next<Index>(target, capture);
                        capture.undo(target);
                        capture.finish(jumper);
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
                        capture.template add_pawn_jump<Color>(jumper);
                }
        }

        template<int Index>
        static bool scan_next(BitBoard jumper, State& capture)
        {
                // tag dispatching on promotion condition
                return scan_next_dispatch<Index>(
                        jumper, capture,
                        Int2Type<rules::promotion_condition<Rules>::value>()
                );
        }

        // partial specialization for pawns that promote apres-fini
        template<int Index>
        static bool scan_next_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<rules::promote_af>
        )
        {
                return (
                        turn<Index>(jumper, capture) |
                        scan<Index>(jumper, capture)
                );
        }

        // partial specialization for pawns that promote en-passant
        template<int Index>
        static bool scan_next_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<rules::promote_ep>
        )
        {
                if (!is_promotion_sq<Color, Board>(jumper))
                        return scan_next_dispatch<Index>(
                                jumper, capture, 
                                Int2Type<rules::promote_af>()
                        );
                else {
                        capture.toggle_promotion();
                        const auto found_next = KingJumps::promote_en_passant<Index>(jumper, capture);
                        capture.toggle_promotion();
                        return found_next;
                }
        }

        template<int Index>
        static bool turn(BitBoard jumper, State& capture)
        {
                // tag dispatching on man turn directions
                return turn_dispatch<Index>(
                        jumper, capture,
                        Int2Type<rules::pawn_turn_directions<Rules>::value>()
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

        // partial specialization for turns in the 1 mirrored forward direction
        template<int Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, 
                Int2Type<rules::turn_up>
        )
        {
                return scan< mirror< angle<Index>, angle<Direction::up> >::value >(jumper, capture);
        }

        // partial specialization for turns in the 1 mirrored backward direction
        template<int Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, Int2Type<rules::turn_down>
        )
        {
                return scan< mirror< angle<Index>, angle<Direction::down> >::value >(jumper, capture);
        }

        template<int Index>
        static bool scan(BitBoard jumper, State& capture)
        {
                PushAssign<Board, Index>()(jumper);
                return jump<Index>(jumper, capture);
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
        static bool detect(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sandwich<Board, Index, rules::scan_1>()(active_pawns, passive_pieces, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
