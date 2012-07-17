#pragma once
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../Selection.hpp"
#include "KingJumps.hpp"                // promote_en_passant
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Degrees.hpp"
#include "../../board/Shift.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for pawn jumps generation
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::pawn, select::Jumps, generation, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material::king, select::Jumps, generation, Rules, Board> KingJumps;
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
                select(p, capture);
        }

private:
        template<typename Position>
        static void select(Position const& p, State& capture)
        {
                // tag dispatching on whether pawns can capture kings
                select_dispatch(p, capture, typename Rules::is_pawns_jump_kings());
        }

        // overload for pawns that can capture kings
        template<typename Position>
        static void select_dispatch(Position const& p, State& capture, boost::mpl::true_)
        {
                branch(p.pawns(Color), capture);
        }

        // overload for pawns that cannot capture kings
        template<typename Position>
        static void select_dispatch(Position const& p, State& capture, boost::mpl::false_)
        {
                capture.toggle_king_targets();
                branch(p.pawns(Color), capture);
                capture.toggle_king_targets();
        }    

        static void branch(BitBoard active_pawns, State& capture)
        {
                // tag dispatching on pawn capture directions
                branch_dispatch(active_pawns, capture, typename Rules::pawn_jump_directions());
        }

        // overload for pawns that capture in the 8 diagonal and orthogonal directions
        static void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::all)
        {
                branch_dispatch(active_pawns, capture, rules::directions::diag());
                branch_dispatch(active_pawns, capture, rules::directions::orth());
        }

        // overload for pawns that capture in the 4 diagonal directions
        static void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::diag)
        {
                branch_dispatch(active_pawns, capture, rules::directions::up  ());
                branch_dispatch(active_pawns, capture, rules::directions::down());
        }

        // overload for pawns that capture in the 2 forward diagonal directions
        static void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::up)
        {
                serialize<typename Compass::left_up >(active_pawns, capture);
                serialize<typename Compass::right_up>(active_pawns, capture);
        }

        // overload for pawns that capture in the 2 backward diagonal directions
        static void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::down)
        {
                serialize<typename Compass::left_down >(active_pawns, capture);
                serialize<typename Compass::right_down>(active_pawns, capture);
        }

        // overload for pawns that capture in the 4 orthogonal directions
        static void branch_dispatch(BitBoard active_pawns, State& capture, rules::directions::orth)
        {
                serialize<typename Compass::left >(active_pawns, capture);
                serialize<typename Compass::right>(active_pawns, capture);
                serialize<typename Compass::up   >(active_pawns, capture);
                serialize<typename Compass::down >(active_pawns, capture);
        }

        template<typename Direction>
        static void serialize(BitBoard active_pawns, State& capture)
        {
                BitIndex jumper;
                for (
                        active_pawns &= Pull<Board, Direction>()(capture.template targets<Direction>());
                        active_pawns;
                        bit::clear_first(active_pawns)
                ) {
                        jumper = bit::get_first(active_pawns);
                        capture.launch(jumper);
                        find_first<Direction>(jumper, capture);
                        capture.finish(jumper);
                }
        }

        template<typename Direction>
        static void find_first(BitBoard jumper, State& capture)
        {
                Board::advance<Direction>(jumper);
                BOOST_ASSERT(bit::is_element(jumper, capture.template targets<Direction>()));
                capture.make(jumper);
                add_jump<Direction>(jumper, capture);   // recursively find more jumps
                capture.undo(jumper);
        }

        template<typename Direction>
        static void add_jump(BitIndex jumper, State& capture)
        {
                Board::advance<Direction>(jumper);
                if (
                        !find_next<Direction>(jumper, capture) &&
                        capture.is_improvement()
                ) {
                        if (capture.improvement_is_strict())
                                capture.improve();
                        capture.template add_pawn_jump<Color>(jumper);
                }
        }

        template<typename Direction>
        static bool find_next(BitIndex jumper, State& capture)
        {
                // tag dispatching on promotion condition
                return find_next_dispatch<Direction>(jumper, capture, typename Rules::pawn_promotion());
        }

        // overload for pawns that promote apres-fini
        template<typename Direction>
        static bool find_next_dispatch(BitIndex jumper, State& capture, rules::promotion::apres_fini)
        {
                return find_next_impl<Direction>(jumper, capture);
        }

        // overload for pawns that promote en-passant
        template<typename Direction>
        static bool find_next_dispatch(BitIndex jumper, State& capture, rules::promotion::en_passant)
        {
                if (!is_promotion_sq<Color, Board>(jumper))
                        return find_next_impl<Direction>(jumper, capture);
                else
                        return promote_en_passant<Direction>(jumper, capture);
        }

        template<typename Direction>
        static bool promote_en_passant(BitIndex jumper, State& capture)
        {
                // tag dispatching on whether pawns can capture kings
                return promote_en_passant_dispatch<Direction>(jumper, capture, typename Rules::is_pawns_jump_kings());
        }

        // overload for pawns that can capture kings
        template<typename Direction>
        static bool promote_en_passant_dispatch(BitIndex jumper, State& capture, boost::mpl::true_)
        {
                capture.toggle_promotion();
                auto const found_next = KingJumps::promote_en_passant<Direction>(jumper, capture);
                capture.toggle_promotion();
                return found_next;
        }

        // overload for pawns that cannot capture kings
        template<typename Direction>
        static bool promote_en_passant_dispatch(BitIndex jumper, State& capture, boost::mpl::false_)
        {
                capture.toggle_promotion();     // no longer a pawn
                capture.toggle_king_targets();  // can now capture kings
                auto const found_next = KingJumps::promote_en_passant<Direction>(jumper, capture);
                capture.toggle_king_targets();  // can no longer capture kings
                capture.toggle_promotion();     // now a pawn again
                return found_next;
        } 

        template<typename Direction>
        static bool find_next_impl(BitIndex jumper, State& capture)
        {
                return (
                        turn<Direction>(jumper, capture) |
                        scan<Direction>(jumper, capture)
                );
        }

        template<typename Direction>
        static bool turn(BitIndex jumper, State& capture)
        {
                // tag dispatching on man turn directions
                return turn_dispatch<Direction>(jumper, capture, typename Rules::pawn_turn_directions());
        }

        // overload for turns in all the 6 non-parallel diagonal and orthogonal directions
        template<typename Direction>
        static bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::all)
        {
                return (
                        turn_dispatch<Direction>(jumper, capture, rules::directions::diag()) |
                        turn_dispatch<Direction>(jumper, capture, rules::directions::orth())
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

        // overload for turns in the 1 mirrored forward direction
        template<typename Direction>
        static bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::up)
        {
                return scan< typename mirror< Direction, typename Compass::up >::type >(jumper, capture);
        }

        // overload for turns in the 1 mirrored backward direction
        template<typename Direction>
        static bool turn_dispatch(BitIndex jumper, State& capture, rules::directions::down)
        {
                return scan< typename mirror< Direction, typename Compass::down >::type >(jumper, capture);
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

        template<typename Direction>
        static bool scan(BitIndex jumper, State& capture)
        {
                Board::advance<Direction>(jumper);
                return jump<Direction>(jumper, capture);
        }

        template<typename Direction>
        static bool jump(BitIndex jumper, State& capture)
        {
                if (!bit::is_element(jumper, capture.template targets<Direction>())) 
                        return false;                   // terminated

                capture.make(jumper);
                add_jump<Direction>(jumper, capture);   // recursively find more jumps
                capture.undo(jumper);                
                return true;                            // not terminated 
        }
};

}       // namespace successor
}       // namespace dctl
