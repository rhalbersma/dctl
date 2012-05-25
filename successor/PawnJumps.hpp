#pragma once
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include <boost/mpl/identity.hpp>       // identity
#include "Driver_fwd.hpp"
#include "KingJumps.hpp"
#include "Selection.hpp"
#include "../bit/Bit.hpp"
#include "../board/Degrees.hpp"
#include "../board/Direction.hpp"
#include "../board/Shift.hpp"
#include "../capture/State.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../node/Targets.hpp"
#include "../rules/Enum.hpp"
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
                select_targets(p, capture);
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
                return detect(p.pawns(Color), targets<Color>(p), not_occupied(p));
        }

private:
        template<template<typename, typename> class Position>
        static void select_targets(Position<Rules, Board> const& p, State& capture)
        {
                // tag dispatching on whether pawns can capture kings
                select_targets_dispatch(
                        p, capture,
                        boost::mpl::identity<typename Rules::is_pawns_jump_kings>()
                );
        }

        // partial specialization for pawns that cannot capture kings
        template<template<typename, typename> class Position>
        static void select_targets_dispatch(
                Position<Rules, Board> const& p, State& capture, 
                boost::mpl::identity<boost::mpl::false_>
        )
        {
                capture.toggle_king_targets();
                select_targets_dispatch(
                        p, capture, 
                        boost::mpl::identity<boost::mpl::true_>()
                );
                capture.toggle_king_targets();
        }

        // partial specialization for pawns that can capture kings
        template<template<typename, typename> class Position>
        static void select_targets_dispatch(
                Position<Rules, Board> const& p, State& capture, 
                boost::mpl::identity<boost::mpl::true_>
        )
        {
                generate(p.pawns(Color), capture);
        }

        static void generate(BitBoard active_pawns, State& capture)
        {
                // tag dispatching on pawn capture directions
                generate_dispatch(
                        active_pawns, capture,
                        boost::mpl::identity<typename Rules::pawn_jump_directions>()
                );
        }

        // partial specialization for pawns that capture in the 8 orthogonal and diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                boost::mpl::identity<rules::directions::all>
        )
        {
                generate_dispatch(active_pawns, capture, boost::mpl::identity<rules::directions::orth>());
                generate_dispatch(active_pawns, capture, boost::mpl::identity<rules::directions::diag>());
        }

        // partial specialization for pawns that capture in the 4 orthogonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                boost::mpl::identity<rules::directions::orth>
        )
        {
                generate<typename Direction::left >(active_pawns, capture);
                generate<typename Direction::right>(active_pawns, capture);
                generate<typename Direction::up   >(active_pawns, capture);
                generate<typename Direction::down >(active_pawns, capture);
        }

        // partial specialization for pawns that capture in the 4 diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                boost::mpl::identity<rules::directions::diag>
        )
        {
                generate_dispatch(active_pawns, capture, boost::mpl::identity<rules::directions::up  >());
                generate_dispatch(active_pawns, capture, boost::mpl::identity<rules::directions::down>());
        }

        // partial specialization for pawns that capture in the 2 forward diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                boost::mpl::identity<rules::directions::up>
        )
        {
                generate<typename Direction::left_up >(active_pawns, capture);
                generate<typename Direction::right_up>(active_pawns, capture);
        }

        // partial specialization for pawns that capture in the 2 backward diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                boost::mpl::identity<rules::directions::down>
        )
        {
                generate<typename Direction::left_down >(active_pawns, capture);
                generate<typename Direction::right_down>(active_pawns, capture);
        }

        static bool detect(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied
        )
        {
                // tag dispatching on pawn capture directions
                return detect_dispatch(
                        active_pawns, passive_pieces, not_occupied,
                        boost::mpl::identity<typename Rules::pawn_jump_directions>()
                );
        }

        // partial specialization for pawns that capture in the 8 orthogonal and diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                boost::mpl::identity<rules::directions::all>
        )
        {
                return (
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, boost::mpl::identity<rules::directions::orth>()) ||
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, boost::mpl::identity<rules::directions::diag>())
                );
        }

        // partial specialization for pawns that capture in the 4 orthogonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                boost::mpl::identity<rules::directions::orth>
        )
        {
                return (
                        detect<typename Direction::left >(active_pawns, passive_pieces, not_occupied) ||
                        detect<typename Direction::right>(active_pawns, passive_pieces, not_occupied) ||
                        detect<typename Direction::up   >(active_pawns, passive_pieces, not_occupied) ||
                        detect<typename Direction::down >(active_pawns, passive_pieces, not_occupied)
                );
        }

        // partial specialization for pawns that capture in the 4 diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                boost::mpl::identity<rules::directions::diag>
        )
        {
                return (
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, boost::mpl::identity<rules::directions::up  >()) ||
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, boost::mpl::identity<rules::directions::down>())
                );
        }

        // partial specialization for pawns that capture in the 2 forward diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                boost::mpl::identity<rules::directions::up>
        )
        {
                return (
                        detect<typename Direction::left_up >(active_pawns, passive_pieces, not_occupied) ||
                        detect<typename Direction::right_up>(active_pawns, passive_pieces, not_occupied)
                );
        }

        // partial specialization for pawns that capture in the 2 backward diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                boost::mpl::identity<rules::directions::down>
        )
        {
                return (
                        detect<typename Direction::left_down >(active_pawns, passive_pieces, not_occupied) ||
                        detect<typename Direction::right_down>(active_pawns, passive_pieces, not_occupied)
                );
        }

        template<typename Index>
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

        template<typename Index>
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

        template<typename Index>
        static bool scan_next(BitBoard jumper, State& capture)
        {
                // tag dispatching on promotion condition
                return scan_next_dispatch<Index>(
                        jumper, capture,
                        boost::mpl::identity<typename Rules::pawn_promotion>()
                );
        }

        // partial specialization for pawns that promote apres-fini
        template<typename Index>
        static bool scan_next_dispatch(
                BitBoard jumper, State& capture, 
                boost::mpl::identity<rules::promotion::apres_fini>
        )
        {
                return (
                        turn<Index>(jumper, capture) |
                        scan<Index>(jumper, capture)
                );
        }

        // partial specialization for pawns that promote en-passant
        template<typename Index>
        static bool scan_next_dispatch(
                BitBoard jumper, State& capture, 
                boost::mpl::identity<rules::promotion::en_passant>
        )
        {
                if (!is_promotion_sq<Color, Board>(jumper))
                        return scan_next_dispatch<Index>(
                                jumper, capture, 
                                boost::mpl::identity<rules::promotion::apres_fini>()
                        );
                else {
                        capture.toggle_promotion();
                        auto const found_next = KingJumps::promote_en_passant<Index>(jumper, capture);
                        capture.toggle_promotion();
                        return found_next;
                }
        }

        template<typename Index>
        static bool turn(BitBoard jumper, State& capture)
        {
                // tag dispatching on man turn directions
                return turn_dispatch<Index>(
                        jumper, capture,
                        boost::mpl::identity<typename Rules::pawn_turn_directions>()
                );
        }

        // partial specialization for turns in all the 6 non-parallel orthogonal and diagonal directions
        template<typename Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, 
                boost::mpl::identity<rules::directions::all>
        )
        {
                return (
                        turn_dispatch<Index>(jumper, capture, boost::mpl::identity<rules::directions::orth>()) |
                        turn_dispatch<Index>(jumper, capture, boost::mpl::identity<rules::directions::diag>())
                );
        }

        // partial specialization for turns in the remaining 4 diagonal or orthogonal directions
        template<typename Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, 
                boost::mpl::identity<rules::directions::orth>
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
                boost::mpl::identity<rules::directions::diag>
        )
        {
                return (
                        scan< typename rotate< Index, angle::R090 >::type >(jumper, capture) |
                        scan< typename rotate< Index, angle::L090 >::type >(jumper, capture)
                );
        }

        // partial specialization for turns in the 1 mirrored forward direction
        template<typename Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, 
                boost::mpl::identity<rules::directions::up>
        )
        {
                return scan< typename mirror< Index, typename Direction::up >::type >(jumper, capture);
        }

        // partial specialization for turns in the 1 mirrored backward direction
        template<typename Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, 
                boost::mpl::identity<rules::directions::down>
        )
        {
                return scan< typename mirror< Index, typename Direction::down >::type >(jumper, capture);
        }

        template<typename Index>
        static bool scan(BitBoard jumper, State& capture)
        {
                PushAssign<Board, Index>()(jumper);
                return jump<Index>(jumper, capture);
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
        static bool detect(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sandwich<Board, Index, rules::range::distance_1>()(active_pawns, passive_pieces, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
