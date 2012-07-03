#pragma once
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include "Driver_fwd.hpp"
#include "KingJumps.hpp"
#include "Selection.hpp"
#include "../bit/Bit.hpp"
#include "../board/Compass.hpp"
#include "../board/Degrees.hpp"
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
                select_targets(p, capture);
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
                return detect(p.pawns(Color), targets<Color>(p), not_occupied(p));
        }

private:
        template<typename Position>
        static void select_targets(Position const& p, State& capture)
        {
                // tag dispatching on whether pawns can capture kings
                select_targets_dispatch(
                        p, capture,
                        typename Rules::is_pawns_jump_kings()
                );
        }

        // overload for pawns that can capture kings
        template<typename Position>
        static void select_targets_dispatch(
                Position const& p, State& capture, 
                boost::mpl::true_
        )
        {
                generate(p.pawns(Color), capture);
        }

        // overload for pawns that cannot capture kings
        template<typename Position>
        static void select_targets_dispatch(
                Position const& p, State& capture, 
                boost::mpl::false_
        )
        {
                capture.toggle_king_targets();
                select_targets_dispatch(
                        p, capture, 
                        boost::mpl::true_()
                );
                capture.toggle_king_targets();
        }

        static void generate(BitBoard active_pawns, State& capture)
        {
                // tag dispatching on pawn capture directions
                generate_dispatch(
                        active_pawns, capture,
                        typename Rules::pawn_jump_directions()
                );
        }

        // overload for pawns that capture in the 8 orthogonal and diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                rules::directions::all
        )
        {
                generate_dispatch(active_pawns, capture, rules::directions::orth());
                generate_dispatch(active_pawns, capture, rules::directions::diag());
        }

        // overload for pawns that capture in the 4 orthogonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                rules::directions::orth
        )
        {
                generate<typename Compass::left >(active_pawns, capture);
                generate<typename Compass::right>(active_pawns, capture);
                generate<typename Compass::up   >(active_pawns, capture);
                generate<typename Compass::down >(active_pawns, capture);
        }

        // overload for pawns that capture in the 4 diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                rules::directions::diag
        )
        {
                generate_dispatch(active_pawns, capture, rules::directions::up  ());
                generate_dispatch(active_pawns, capture, rules::directions::down());
        }

        // overload for pawns that capture in the 2 forward diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                rules::directions::up
        )
        {
                generate<typename Compass::left_up >(active_pawns, capture);
                generate<typename Compass::right_up>(active_pawns, capture);
        }

        // overload for pawns that capture in the 2 backward diagonal directions
        static void generate_dispatch(
                BitBoard active_pawns, State& capture, 
                rules::directions::down
        )
        {
                generate<typename Compass::left_down >(active_pawns, capture);
                generate<typename Compass::right_down>(active_pawns, capture);
        }

        static bool detect(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied
        )
        {
                // tag dispatching on pawn capture directions
                return detect_dispatch(
                        active_pawns, passive_pieces, not_occupied,
                        typename Rules::pawn_jump_directions()
                );
        }

        // overload for pawns that capture in the 8 orthogonal and diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                rules::directions::all
        )
        {
                return (
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::orth()) ||
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::diag())
                );
        }

        // overload for pawns that capture in the 4 orthogonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                rules::directions::orth
        )
        {
                return (
                        detect<typename Compass::left >(active_pawns, passive_pieces, not_occupied) ||
                        detect<typename Compass::right>(active_pawns, passive_pieces, not_occupied) ||
                        detect<typename Compass::up   >(active_pawns, passive_pieces, not_occupied) ||
                        detect<typename Compass::down >(active_pawns, passive_pieces, not_occupied)
                );
        }

        // overload for pawns that capture in the 4 diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                rules::directions::diag
        )
        {
                return (
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::up  ()) ||
                        detect_dispatch(active_pawns, passive_pieces, not_occupied, rules::directions::down())
                );
        }

        // overload for pawns that capture in the 2 forward diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                rules::directions::up
        )
        {
                return (
                        detect<typename Compass::left_up >(active_pawns, passive_pieces, not_occupied) ||
                        detect<typename Compass::right_up>(active_pawns, passive_pieces, not_occupied)
                );
        }

        // overload for pawns that capture in the 2 backward diagonal directions
        static bool detect_dispatch(
                BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied, 
                rules::directions::down
        )
        {
                return (
                        detect<typename Compass::left_down >(active_pawns, passive_pieces, not_occupied) ||
                        detect<typename Compass::right_down>(active_pawns, passive_pieces, not_occupied)
                );
        }

        template<typename Direction>
        static void generate(BitBoard active_pawns, State& capture)
        {
                BitIndex jumper, target;
                for (
                        active_pawns &= Pull<Board, Direction>()(capture.template targets<Direction>());
                        active_pawns;
                        bit::clear_first(active_pawns)
                ) {
                        jumper = bit::get_first(active_pawns);
                        capture.launch(jumper);
                        target = Board::next<Direction>(jumper);
                        capture.make(target);
                        generate_next<Direction>(target, capture);
                        capture.undo(target);
                        capture.finish(jumper);
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
                        capture.template add_pawn_jump<Color>(jumper);
                }
        }

        template<typename Direction>
        static bool scan_next(BitIndex jumper, State& capture)
        {
                // tag dispatching on promotion condition
                return scan_next_dispatch<Direction>(
                        jumper, capture,
                        typename Rules::pawn_promotion()
                );
        }

        // overload for pawns that promote apres-fini
        template<typename Direction>
        static bool scan_next_dispatch(
                BitIndex jumper, State& capture, 
                rules::promotion::apres_fini
        )
        {
                return (
                        turn<Direction>(jumper, capture) |
                        scan<Direction>(jumper, capture)
                );
        }

        // overload for pawns that promote en-passant
        template<typename Direction>
        static bool scan_next_dispatch(
                BitIndex jumper, State& capture, 
                rules::promotion::en_passant
        )
        {
                if (!is_promotion_sq<Color, Board>(jumper))
                        return scan_next_dispatch<Direction>(
                                jumper, capture, 
                                rules::promotion::apres_fini()
                        );
                else {
                        capture.toggle_promotion();
                        auto const found_next = KingJumps::promote_en_passant<Direction>(jumper, capture);
                        capture.toggle_promotion();
                        return found_next;
                }
        }

        template<typename Direction>
        static bool turn(BitIndex jumper, State& capture)
        {
                // tag dispatching on man turn directions
                return turn_dispatch<Direction>(
                        jumper, capture,
                        typename Rules::pawn_turn_directions()
                );
        }

        // overload for turns in all the 6 non-parallel orthogonal and diagonal directions
        template<typename Direction>
        static bool turn_dispatch(
                BitIndex jumper, State& capture, 
                rules::directions::all
        )
        {
                return (
                        turn_dispatch<Direction>(jumper, capture, rules::directions::orth()) |
                        turn_dispatch<Direction>(jumper, capture, rules::directions::diag())
                );
        }

        // overload for turns in the remaining 4 diagonal or orthogonal directions
        template<typename Direction>
        static bool turn_dispatch(
                BitIndex jumper, State& capture, 
                rules::directions::orth
        )
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
        static bool turn_dispatch(
                BitIndex jumper, State& capture, 
                rules::directions::diag
        )
        {
                return (
                        scan< typename rotate< Direction, angle::R090 >::type >(jumper, capture) |
                        scan< typename rotate< Direction, angle::L090 >::type >(jumper, capture)
                );
        }

        // overload for turns in the 1 mirrored forward direction
        template<typename Direction>
        static bool turn_dispatch(
                BitIndex jumper, State& capture, 
                rules::directions::up
        )
        {
                return scan< typename mirror< Direction, typename Compass::up >::type >(jumper, capture);
        }

        // overload for turns in the 1 mirrored backward direction
        template<typename Direction>
        static bool turn_dispatch(
                BitIndex jumper, State& capture, 
                rules::directions::down
        )
        {
                return scan< typename mirror< Direction, typename Compass::down >::type >(jumper, capture);
        }

        template<typename Direction>
        static bool scan(BitIndex jumper, State& capture)
        {
                Board::advance<Direction>(jumper);
                return jump<Direction>(jumper, capture);
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
        static bool detect(BitBoard active_pawns, BitBoard passive_pieces, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sandwich<Board, Direction, rules::range::distance_1>()(active_pawns, passive_pieces, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
