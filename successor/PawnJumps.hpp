#pragma once
#include "../bit/Bit.hpp"
#include "../board/Angle.hpp"
#include "../board/Board.h"
#include "../board/Direction.hpp"
#include "../capture/State.hpp"
#include "../node/Material.h"
#include "../node/Position.h"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/Shift.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {
namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
class Jumps;

// partial specialization for pawn jumps
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::PAWN, Jumps, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        // typedefs
        typedef Driver<Color, Material::KING, Jumps, Rules, Board> KingJumps;
        typedef board::Direction<Color, Board> Direction;
        typedef capture::State<Rules, Board> State;

        static void generate(const Position<Board>& p, Stack& moves)
        {
                State capture(p);
                generate(p, capture, moves);
        }

        static void generate(const Position<Board>& p, State& capture, Stack& moves)
        {
                generate_targets(p, capture, moves);
        }

        static int count(const Position<Board>& p)
        {
                Stack moves;
                generate(p, moves);
                return static_cast<int>(moves.size());
        }

        static bool detect(const Position<Board>& p)
        {
                return detect_targets(p);
        }

private:        
        // tag dispatching on whether men can capture kings
        static void generate_targets(const Position<Board>& p, State& capture, Stack& moves)
        {
                generate_targets(
                        p, capture, moves, 
                        Int2Type<rules::is_men_capture_kings<Rules>::value>()
                );
        }
        
        // partial specialization for men that cannot capture kings
        static void generate_targets(
                const Position<Board>& p, State& capture, Stack& moves, Int2Type<false>
        )
        {
                capture.toggle_king_targets();
                generate_targets(p, capture, moves, Int2Type<true>());
                capture.toggle_king_targets();
        }

        // partial specialization for men that can capture kings
        static void generate_targets(
                const Position<Board>& p, State& capture, Stack& moves, Int2Type<true>
        )
        {
                generate_dirs(p.men(Color), capture, moves);
        }

        // tag dispatching on whether men can capture kings
        static bool detect_targets(const Position<Board>& p)
        {
                return detect_targets(p, Int2Type<rules::is_men_capture_kings<Rules>::value>());
        }

        // partial specialization for men that cannot capture kings
        static bool detect_targets(
                const Position<Board>& p, Int2Type<false>
        )
        {
                return detect_dirs(p.men(Color), p.men(!Color), not_occupied(p));
        }

        // partial specialization for men that can capture kings
        static bool detect_targets(
                const Position<Board>& p, Int2Type<true>
        )
        {
                return detect_dirs(p.men(Color), p.pieces(!Color), not_occupied(p));
        }

        // tag dispatching on man capture directions
        static void generate_dirs(BitBoard active_men, State& capture, Stack& moves)
        {
                generate_dirs(
                        active_men, capture, moves, 
                        Int2Type<rules::man_capture_directions<Rules>::value>()
                );
        }

        // partial specialization for men that capture in the 8 orthogonal and diagonal directions
        static void generate_dirs(
                BitBoard active_men, State& capture, Stack& moves, Int2Type<rules::dirs_all>
        )
        {
                generate_dirs(active_men, capture, moves, Int2Type<rules::dirs_orth>());
                generate_dirs(active_men, capture, moves, Int2Type<rules::dirs_diag>());
        }

        // partial specialization for men that capture in the 4 orthogonal directions
        static void generate_dirs(
                BitBoard active_men, State& capture, Stack& moves, Int2Type<rules::dirs_orth>
        )
        {
                generate_dir<Direction::left >(active_men, capture, moves);
                generate_dir<Direction::right>(active_men, capture, moves);
                generate_dir<Direction::up   >(active_men, capture, moves);
                generate_dir<Direction::down >(active_men, capture, moves);
        }

        // partial specialization for men that capture in the 4 diagonal directions
        static void generate_dirs(
                BitBoard active_men, State& capture, Stack& moves, Int2Type<rules::dirs_diag>
        )
        {
                generate_dirs(active_men, capture, moves, Int2Type<rules::dirs_up  >());
                generate_dirs(active_men, capture, moves, Int2Type<rules::dirs_down>());
        }

        // partial specialization for men that capture in the 2 forward diagonal directions
        static void generate_dirs(
                BitBoard active_men, State& capture, Stack& moves, Int2Type<rules::dirs_up>
        )
        {
                generate_dir<Direction::left_up >(active_men, capture, moves);
                generate_dir<Direction::right_up>(active_men, capture, moves);
        }

        // partial specialization for men that capture in the 2 backward diagonal directions
        static void generate_dirs(
                BitBoard active_men, State& capture, Stack& moves, Int2Type<rules::dirs_down>
        )
        {
                generate_dir<Direction::left_down >(active_men, capture, moves);
                generate_dir<Direction::right_down>(active_men, capture, moves);
        }

        // tag dispatching on man capture directions
        static bool detect_dirs(
                BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied
        )
        {
                return detect_dirs(
                        active_men, opponent_pieces, not_occupied, 
                        Int2Type<rules::man_capture_directions<Rules>::value>()
                );
        }

        // partial specialization for men that capture in the 8 orthogonal and diagonal directions
        static bool detect_dirs(
                BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_all>
        )
        {
                return (
                        detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::dirs_orth>()) ||
                        detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::dirs_diag>())
                );
        }
        
        // partial specialization for men that capture in the 4 orthogonal directions
        static bool detect_dirs(
                BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_orth>
        )
        {
                return (
                        detect_dir<Direction::left >(active_men, opponent_pieces, not_occupied) ||
                        detect_dir<Direction::right>(active_men, opponent_pieces, not_occupied) ||
                        detect_dir<Direction::up   >(active_men, opponent_pieces, not_occupied) ||
                        detect_dir<Direction::down >(active_men, opponent_pieces, not_occupied)
                );
        }
        
        // partial specialization for men that capture in the 4 diagonal directions
        static bool detect_dirs(
                BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_diag>
        )
        {
                return (
                        detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::dirs_up  >()) ||
                        detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::dirs_down>())
                );
        }
        
        // partial specialization for men that capture in the 2 forward diagonal directions
        static bool detect_dirs(
                BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_up>
        )
        {
                return (
                        detect_dir<Direction::left_up >(active_men, opponent_pieces, not_occupied) ||
                        detect_dir<Direction::right_up>(active_men, opponent_pieces, not_occupied)
                );
        }
        
        // partial specialization for men that capture in the 2 backward diagonal directions
        static bool detect_dirs(
                BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_down>
        )
        {
                return (
                        detect_dir<Direction::left_down >(active_men, opponent_pieces, not_occupied) ||
                        detect_dir<Direction::right_down>(active_men, opponent_pieces, not_occupied)
                );
        }

        template<int Index> 
        static void generate_dir(BitBoard active_men, State& capture, Stack& moves)
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

        // tag dispatching on promotion condition
        template<int Index>
        static bool scan_next(BitBoard jumper, State& capture, Stack& moves)
        {
                return scan_next<Index>(
                        jumper, capture, moves, 
                        Int2Type<rules::promotion_condition<Rules>::value>()
                );
        }
        
        // partial specialization for men that promote en-passant
        template<int Index>
        static bool scan_next(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::promote_ep>
        )
        {
                if (!capture.is_promotion_sq<Color>(jumper))
                        return scan_next<Index>(jumper, capture, moves, Int2Type<rules::promote_af>());
                else {
                        capture.toggle_promotion();
                        const bool found_next = KingJumps::promote_en_passant<Index>(jumper, capture, moves);
                        capture.toggle_promotion();
                        return found_next;
                }
        }

        // partial specialization for men that promote apres-fini
        template<int Index>
        static bool scan_next(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::promote_af>
        )
        {
                return (
                        scan_dirs<Index>(jumper, capture, moves) |
                        scan_dir<Index>(jumper, capture, moves)
                );
        }
        
        // tag dispatching on man scan directions
        template<int Index>
        static bool scan_dirs(BitBoard jumper, State& capture, Stack& moves)
        {
                return scan_dirs<Index>(
                        jumper, capture, moves, 
                        Int2Type<rules::man_scan_directions<Rules>::value>()
                );
        }
        
        // partial specialization for scans in all the 6 non-parallel orthogonal and diagonal directions
        template<int Index>
        static bool scan_dirs(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::scan_all>
        )
        {
                return (
                        scan_dirs<Index>(jumper, capture, moves, Int2Type<rules::scan_orth>()) |
                        scan_dirs<Index>(jumper, capture, moves, Int2Type<rules::scan_diag>())
                );
        }
        
        // partial specialization for scans in the remaining 4 diagonal or orthogonal directions
        template<int Index>
                static bool scan_dirs(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::scan_orth>
        )
        {
                return (
                        scan_dir<rotate<Angle<Index>, Degrees::R045>::type::value>(jumper, capture, moves) |
                        scan_dir<rotate<Angle<Index>, Degrees::L045>::type::value>(jumper, capture, moves) |
                        scan_dir<rotate<Angle<Index>, Degrees::R135>::type::value>(jumper, capture, moves) |
                        scan_dir<rotate<Angle<Index>, Degrees::L135>::type::value>(jumper, capture, moves)
                );
        }
        
        // partial specialization for scans in the 2 sideways directions
        template<int Index>
        static bool scan_dirs(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::scan_diag>
        )
        {
                return (
                        scan_dir<rotate<Angle<Index>, Degrees::R090>::type::value>(jumper, capture, moves) |
                        scan_dir<rotate<Angle<Index>, Degrees::L090>::type::value>(jumper, capture, moves)
                );
        }
        
        // partial specialization for scans in the 1 mirrored forward direction
        template<int Index>
        static bool scan_dirs(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::scan_up>
        )
        {
                return scan_dir<mirror_up<Index>::value>(jumper, capture, moves);
        }
        
        // partial specialization for scans in the 1 mirrored backward direction
        template<int Index>
        static bool scan_dirs(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::scan_down>
        )
        {
                return scan_dir<mirror_down<Index>::value>(jumper, capture, moves);
        }

        template<int Index>
        static bool scan_dir(BitBoard jumper, State& capture, Stack& moves)
        {
                PushAssign<Board, Index>()(jumper);
                return scan<Index>(jumper, capture, moves);
        }
        
        template<int Index>
        static bool scan(
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
        static bool detect_dir(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Push<Board, Index>()(active_men) & 
                        opponent_pieces & 
                        Pull<Board, Index>()(not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
