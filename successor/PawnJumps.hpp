#pragma once
#include "Driver_fwd.hpp"
#include "KingJumps.hpp"
#include "Selection.hpp"
#include "../bit/Bit.hpp"
#include "../board/Angle.hpp"
#include "../board/Board.hpp"
#include "../board/Direction.hpp"
#include "../capture/State.hpp"
#include "../node/Material.hpp"
#include "../node/Position.hpp"
#include "../node/Stack.hpp"
#include "../node/Targets.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/NonConstructible.hpp"
#include "../utility/Shift.hpp"

namespace dctl {
namespace successor {

// partial specialization for pawn jumps
template<bool Color, typename Rules, typename Board> 
struct Driver<Color, Material::pawn, select::Jumps, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
private:
        // typedefs
        typedef Driver<Color, Material::king, select::Jumps, Rules, Board> KingJumps;
        typedef board::Direction<Color, Board> Direction;
        typedef capture::State<Rules, Board> State;

public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                State capture(p);
                generate(p, capture, moves);
        }

        static void generate(const Position<Board>& p, State& capture, Stack& moves)
        {
                select_targets(p, capture, moves);
        }

        static int count(const Position<Board>& p)
        {
                Stack moves;
                generate(p, moves);
                return static_cast<int>(moves.size());
        }

        static bool detect(const Position<Board>& p)
        {
                return detect(p.men(Color), targets<Color, Rules>(p), not_occupied(p));
        }

private:        
        static void select_targets(const Position<Board>& p, State& capture, Stack& moves)
        {
                // tag dispatching on whether men can capture kings
                select_targets_dispatch(
                        p, capture, moves, 
                        Int2Type<rules::is_men_capture_kings<Rules>::value>()
                );
        }
        
        // partial specialization for men that cannot capture kings
        static void select_targets_dispatch(
                const Position<Board>& p, State& capture, Stack& moves, Int2Type<false>
        )
        {
                capture.toggle_king_targets();
                select_targets_dispatch(p, capture, moves, Int2Type<true>());
                capture.toggle_king_targets();
        }

        // partial specialization for men that can capture kings
        static void select_targets_dispatch(
                const Position<Board>& p, State& capture, Stack& moves, Int2Type<true>
        )
        {
                generate(p.men(Color), capture, moves);
        }

        static void generate(BitBoard active_men, State& capture, Stack& moves)
        {
                // tag dispatching on man capture directions
                generate_dispatch(
                        active_men, capture, moves, 
                        Int2Type<rules::man_capture_directions<Rules>::value>()
                );
        }

        // partial specialization for men that capture in the 8 orthogonal and diagonal directions
        static void generate_dispatch(
                BitBoard active_men, State& capture, Stack& moves, Int2Type<rules::dirs_all>
        )
        {
                generate_dispatch(active_men, capture, moves, Int2Type<rules::dirs_orth>());
                generate_dispatch(active_men, capture, moves, Int2Type<rules::dirs_diag>());
        }

        // partial specialization for men that capture in the 4 orthogonal directions
        static void generate_dispatch(
                BitBoard active_men, State& capture, Stack& moves, Int2Type<rules::dirs_orth>
        )
        {
                generate<Direction::left >(active_men, capture, moves);
                generate<Direction::right>(active_men, capture, moves);
                generate<Direction::up   >(active_men, capture, moves);
                generate<Direction::down >(active_men, capture, moves);
        }

        // partial specialization for men that capture in the 4 diagonal directions
        static void generate_dispatch(
                BitBoard active_men, State& capture, Stack& moves, Int2Type<rules::dirs_diag>
        )
        {
                generate_dispatch(active_men, capture, moves, Int2Type<rules::dirs_up  >());
                generate_dispatch(active_men, capture, moves, Int2Type<rules::dirs_down>());
        }

        // partial specialization for men that capture in the 2 forward diagonal directions
        static void generate_dispatch(
                BitBoard active_men, State& capture, Stack& moves, Int2Type<rules::dirs_up>
        )
        {
                generate<Direction::left_up >(active_men, capture, moves);
                generate<Direction::right_up>(active_men, capture, moves);
        }

        // partial specialization for men that capture in the 2 backward diagonal directions
        static void generate_dispatch(
                BitBoard active_men, State& capture, Stack& moves, Int2Type<rules::dirs_down>
        )
        {
                generate<Direction::left_down >(active_men, capture, moves);
                generate<Direction::right_down>(active_men, capture, moves);
        }

        static bool detect(
                BitBoard active_men, BitBoard passive_pieces, BitBoard not_occupied
        )
        {
                // tag dispatching on man capture directions
                return detect_dispatch(
                        active_men, passive_pieces, not_occupied, 
                        Int2Type<rules::man_capture_directions<Rules>::value>()
                );
        }

        // partial specialization for men that capture in the 8 orthogonal and diagonal directions
        static bool detect_dispatch(
                BitBoard active_men, BitBoard passive_pieces, BitBoard not_occupied, Int2Type<rules::dirs_all>
        )
        {
                return (
                        detect_dispatch(active_men, passive_pieces, not_occupied, Int2Type<rules::dirs_orth>()) ||
                        detect_dispatch(active_men, passive_pieces, not_occupied, Int2Type<rules::dirs_diag>())
                );
        }
        
        // partial specialization for men that capture in the 4 orthogonal directions
        static bool detect_dispatch(
                BitBoard active_men, BitBoard passive_pieces, BitBoard not_occupied, Int2Type<rules::dirs_orth>
        )
        {
                return (
                        detect<Direction::left >(active_men, passive_pieces, not_occupied) ||
                        detect<Direction::right>(active_men, passive_pieces, not_occupied) ||
                        detect<Direction::up   >(active_men, passive_pieces, not_occupied) ||
                        detect<Direction::down >(active_men, passive_pieces, not_occupied)
                );
        }
        
        // partial specialization for men that capture in the 4 diagonal directions
        static bool detect_dispatch(
                BitBoard active_men, BitBoard passive_pieces, BitBoard not_occupied, Int2Type<rules::dirs_diag>
        )
        {
                return (
                        detect_dispatch(active_men, passive_pieces, not_occupied, Int2Type<rules::dirs_up  >()) ||
                        detect_dispatch(active_men, passive_pieces, not_occupied, Int2Type<rules::dirs_down>())
                );
        }
        
        // partial specialization for men that capture in the 2 forward diagonal directions
        static bool detect_dispatch(
                BitBoard active_men, BitBoard passive_pieces, BitBoard not_occupied, Int2Type<rules::dirs_up>
        )
        {
                return (
                        detect<Direction::left_up >(active_men, passive_pieces, not_occupied) ||
                        detect<Direction::right_up>(active_men, passive_pieces, not_occupied)
                );
        }
        
        // partial specialization for men that capture in the 2 backward diagonal directions
        static bool detect_dispatch(
                BitBoard active_men, BitBoard passive_pieces, BitBoard not_occupied, Int2Type<rules::dirs_down>
        )
        {
                return (
                        detect<Direction::left_down >(active_men, passive_pieces, not_occupied) ||
                        detect<Direction::right_down>(active_men, passive_pieces, not_occupied)
                );
        }

        template<int Index> 
        static void generate(BitBoard active_men, State& capture, Stack& moves)
        {
                BitBoard jumper, target;
                for (
                        active_men &= Pull<Board, Index>()(capture.template targets<Index>()); 
                        active_men; 
                        bit::clear_first(active_men)
                ) {
                        jumper = bit::get_first(active_men);
                        capture.launch(jumper);
                        target = Push<Board, Index>()(jumper);
                        capture.make(target);
                        generate_next<Index>(target, capture, moves);
                        capture.undo(target);
                        capture.finish(jumper);
                }
        }

        template<int Index> 
        static void generate_next(BitBoard jumper, State& capture, Stack& moves)
        {
                PushAssign<Board, Index>()(jumper);
                if (
                        !scan_next<Index>(jumper, capture, moves) && 
                        (capture.current() >= capture.best())
                ) {
		        if (capture.current() != capture.best()) {
                                capture.best() = capture.current();
                                moves.clear();                
                        }
		        capture.template add_man_capture<Color>(jumper, moves);
	        }
        }

        template<int Index>
        static bool scan_next(BitBoard jumper, State& capture, Stack& moves)
        {
                // tag dispatching on promotion condition
                return scan_next_dispatch<Index>(
                        jumper, capture, moves, 
                        Int2Type<rules::promotion_condition<Rules>::value>()
                );
        }
        
        // partial specialization for men that promote apres-fini
        template<int Index>
        static bool scan_next_dispatch(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::promote_af>
        )
        {
                return (
                        turn<Index>(jumper, capture, moves) |
                        scan<Index>(jumper, capture, moves)
                );
        }
        
        // partial specialization for men that promote en-passant
        template<int Index>
        static bool scan_next_dispatch(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::promote_ep>
        )
        {
                if (!capture.is_promotion_sq<Color>(jumper))
                        return scan_next_dispatch<Index>(jumper, capture, moves, Int2Type<rules::promote_af>());
                else {
                        capture.current_toggle_promotion();
                        const auto found_next = KingJumps::promote_en_passant<Index>(jumper, capture, moves);
                        capture.current_toggle_promotion();
                        return found_next;
                }
        }

        template<int Index>
        static bool turn(BitBoard jumper, State& capture, Stack& moves)
        {
                // tag dispatching on man turn directions
                return turn_dispatch<Index>(
                        jumper, capture, moves, 
                        Int2Type<rules::man_turn_directions<Rules>::value>()
                );
        }
        
        // partial specialization for turns in all the 6 non-parallel orthogonal and diagonal directions
        template<int Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::turn_all>
        )
        {
                return (
                        turn_dispatch<Index>(jumper, capture, moves, Int2Type<rules::turn_orth>()) |
                        turn_dispatch<Index>(jumper, capture, moves, Int2Type<rules::turn_diag>())
                );
        }
        
        // partial specialization for turns in the remaining 4 diagonal or orthogonal directions
        template<int Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::turn_orth>
        )
        {
                return (
                        scan< rotate< angle<Index>, angle<degrees::R045> >::value >(jumper, capture, moves) |
                        scan< rotate< angle<Index>, angle<degrees::L045> >::value >(jumper, capture, moves) |
                        scan< rotate< angle<Index>, angle<degrees::R135> >::value >(jumper, capture, moves) |
                        scan< rotate< angle<Index>, angle<degrees::L135> >::value >(jumper, capture, moves)
                );
        }
        
        // partial specialization for turns in the 2 sideways directions
        template<int Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::turn_diag>
        )
        {
                return (
                        scan< rotate< angle<Index>, angle<degrees::R090> >::value >(jumper, capture, moves) |
                        scan< rotate< angle<Index>, angle<degrees::L090> >::value >(jumper, capture, moves)
                );
        }
        
        // partial specialization for turns in the 1 mirrored forward direction
        template<int Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::turn_up>
        )
        {
                return scan< mirror< angle<Index>, angle<Direction::up> >::value >(jumper, capture, moves);
        }
        
        // partial specialization for turns in the 1 mirrored backward direction
        template<int Index>
        static bool turn_dispatch(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::turn_down>
        )
        {
                return scan< mirror< angle<Index>, angle<Direction::down> >::value >(jumper, capture, moves);
        }

        template<int Index>
        static bool scan(BitBoard jumper, State& capture, Stack& moves)
        {
                PushAssign<Board, Index>()(jumper);
                return jump<Index>(jumper, capture, moves);
        }
        
        template<int Index>
        static bool jump(
                BitBoard jumper, State& capture, Stack& moves
        )
        {
                if (jumper & capture.template targets<Index>()) {
                        capture.make(jumper);
                        generate_next<Index>(jumper, capture, moves);
                        capture.undo(jumper);
                        return true;
                } else
                        return false;
        }

        template<int Index>
        static bool detect(BitBoard active_men, BitBoard passive_pieces, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sandwich<Board, Index, rules::scan_1>()(active_men, passive_pieces, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
