#pragma once
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include "../bit/Bit.hpp"
#include "../board/Angle.hpp"
#include "../board/Direction.hpp"
#include "../capture/State.hpp"
#include "../node/Material.hpp"
#include "../node/Position.hpp"
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

// partial specialization for king jumps
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::king, Jumps, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        // typedefs
        typedef board::Direction<Color, Board> Direction;
        typedef capture::State<Rules, Board> State;

        static void generate(const Position<Board>& p, Stack& moves)
        {
                State capture(p);
                generate(p, capture, moves);
        }

        static void generate(const Position<Board>& p, State& capture, Stack& moves)
        {
                generate_precede(p, capture, moves);
        }

        template<int Index> 
        static bool promote_en_passant(BitBoard jumper, State& capture, Stack& moves)
        {
                return scan_next<Index>(jumper, capture, moves);
        }

        static int count(const Position<Board>& p)
        {
                Stack moves;
                generate(p, moves);
                return static_cast<int>(moves.size());
        }

        static bool detect(const Position<Board>& p)
        {
                return detect_dirs(p.kings(Color), p.pieces(!Color), not_occupied(p));
        }

private:
        // tag dispatching on relative king capture precedence
        static void generate_precede(
        const Position<Board>& p, State& capture, Stack& moves)
        {
                generate_precede(
                        p, capture, moves, 
                        Int2Type<rules::is_relative_king_precedence<Rules>::value>()
                );
        }

        // partial specialization for relative king capture precedence
        static void generate_precede(
                const Position<Board>& p, State& capture, Stack& moves, Int2Type<true>
        )
        {
                capture.toggle_with_king();
                generate_precede(p, capture, moves, Int2Type<false>());
                capture.toggle_with_king();
        }
        
        // partial specialization for no relative king capture precedence
        static void generate_precede(
                const Position<Board>& p, State& capture, Stack& moves, Int2Type<false>
        )
        {
                generate_serial(p.kings(Color), capture, moves);
        }

        static void generate_serial(BitBoard active_kings, State& capture, Stack& moves)
        {
                BitBoard jumper;
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        jumper = bit::get_first(active_kings);
                        capture.launch(jumper);
                        generate_dirs(jumper, capture, moves);
                        capture.finish(jumper);
                        bit::clear_first(active_kings);
                } while (active_kings);
        }

        // tag dispatching on king capture directions
        static void generate_dirs(BitBoard jumper, State& capture, Stack& moves)
        {
                generate_dirs(
                        jumper, capture, moves, 
                        Int2Type<rules::king_capture_directions<Rules>::value>()
                );
        }

        // partial specialization for kings that capture in the 8 orthogonal and diagonal directions
        static void generate_dirs(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::dirs_all>
        )
        {
                generate_dirs(jumper, capture, moves, Int2Type<rules::dirs_orth>());
                generate_dirs(jumper, capture, moves, Int2Type<rules::dirs_diag>());
        }

        // partial specialization for kings that capture in the 4 orthogonal directions
        static void generate_dirs(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::dirs_orth>
        )
        {
                generate_dir<Direction::left >(jumper, capture, moves);
                generate_dir<Direction::right>(jumper, capture, moves);
                generate_dir<Direction::up   >(jumper, capture, moves);
                generate_dir<Direction::down >(jumper, capture, moves);
        }
        
        // partial specialization for kings that capture in the 4 diagonal directions
        static void generate_dirs(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::dirs_diag>
        )
        {
                generate_dir<Direction::left_up   >(jumper, capture, moves);
                generate_dir<Direction::right_up  >(jumper, capture, moves);
                generate_dir<Direction::left_down >(jumper, capture, moves);
                generate_dir<Direction::right_down>(jumper, capture, moves);
        }

        // tag dispatching on king capture directions
        static bool detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
        {
                return detect_dirs(
                        active_kings, opponent_pieces, not_occupied, 
                        Int2Type<rules::king_capture_directions<Rules>::value>()
                );
        }
        
        // partial specialization for kings that capture in the 8 orthogonal and diagonal directions
        static bool detect_dirs(
                BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_all>
        )
        {
                return (
                        detect_dirs(active_kings, opponent_pieces, not_occupied, Int2Type<rules::dirs_orth>()) ||
                        detect_dirs(active_kings, opponent_pieces, not_occupied, Int2Type<rules::dirs_diag>())
                );
        }
        
        // partial specialization for kings that capture in the 4 orthogonal directions
        static bool detect_dirs(
                BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_orth>
        )
        {
                return (
                        detect_dir<Direction::left >(active_kings, opponent_pieces, not_occupied) ||
                        detect_dir<Direction::right>(active_kings, opponent_pieces, not_occupied) ||
                        detect_dir<Direction::up   >(active_kings, opponent_pieces, not_occupied) ||
                        detect_dir<Direction::down >(active_kings, opponent_pieces, not_occupied)
                );
        }
        
        // partial specialization for kings that capture in the 4 diagonal directions
        static bool detect_dirs(
                BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_diag>
        )
        {
                return (
                        detect_dir<Direction::left_up   >(active_kings, opponent_pieces, not_occupied) ||
                        detect_dir<Direction::right_up  >(active_kings, opponent_pieces, not_occupied) ||
                        detect_dir<Direction::left_down >(active_kings, opponent_pieces, not_occupied) ||
                        detect_dir<Direction::right_down>(active_kings, opponent_pieces, not_occupied)
                );
        }

        template<int Index> 
        static void generate_dir(BitBoard jumper, State& capture, Stack& moves)
        {
                king_slide<Index>(jumper, capture.template path<Index>());
                if (jumper & capture.template targets<Index>()) {
                        capture.make(jumper);
                        generate_next<Index>(jumper, capture, moves);
                        capture.undo(jumper);
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
		        capture.template add_king_capture<Color, Index>(jumper, moves);
                }
        }

        // tag dispatching on king range
        template<int Index> 
        static bool scan_next(BitBoard jumper, State& capture, Stack& moves)
        {
                return scan_next<Index>(
                        jumper, capture, moves, 
                        Int2Type<rules::is_long_king_range<Rules>::value>()
                );
        }
        
        // partial specialization for short ranged kings
        template<int Index> 
        static bool scan_next(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::range_1>
        )
        {
                return (
                        scan_dirs<Index>(jumper, capture, moves) |
                        scan_dir<Index>(jumper, capture, moves)
                );
        }
        
        // partial specialization for long ranged kings
        template<int Index> 
        static bool scan_next(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<rules::range_N>
        )
        {
                return scan_long<Index>(jumper, capture, moves);
        }

        // tag dispatching on king capture direction reversal
        template<int Index> 
        static bool scan_long(BitBoard jumper, State& capture, Stack& moves)
        {
                return scan_long<Index>(
                        jumper, capture, moves, 
                        Int2Type<rules::is_capture_direction_reversal<Rules>::value>()
                );
        }
        
        // partial specialization for kings that cannot reverse their capture direction
        template<int Index> 
        static bool scan_long(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<false>
        )
        {
                return scan_forward<Index>(jumper, capture, moves);
        }
        
        // partial specialization for kings that can reverse their capture direction
        template<int Index> 
        static bool scan_long(
                BitBoard jumper, State& capture, Stack& moves, Int2Type<true>
        )
        {
                return (
                        scan_reverse<Index>(jumper, capture, moves) |
                        scan_forward<Index>(jumper, capture, moves)
                );
        }

        template<int Index> 
        static bool scan_reverse(BitBoard jumper, State& capture, Stack& moves)
        {
                return scan_dir<rotate<Angle<Index>, Degrees::D180>::type::value>(jumper, capture, moves);
        }
        
        template<int Index> 
        static bool scan_forward(BitBoard jumper, State& capture, Stack& moves)
        {
                BOOST_ASSERT(jumper & capture.path());
                bool found_capture = false;
                do {
                        found_capture |= scan_dirs<Index>(jumper, capture, moves);
                        PushAssign<Board, Index>()(jumper);
                } while (jumper & capture.path());
                return found_capture |= scan<Index>(jumper, capture, moves);
        }

        // tag dispatching on king scan directions
        template<int Index> 
        static bool scan_dirs(BitBoard jumper, State& capture, Stack& moves)
        {
                return scan_dirs<Index>(
                        jumper, capture, moves, 
                        Int2Type<rules::king_scan_directions<Rules>::value>()
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

        template<int Index> 
        static bool scan_dir(BitBoard jumper, State& capture, Stack& moves)
        {
                king_slide<Index>(jumper, capture.template path<Index>());
                return scan<Index>(jumper, capture, moves);
        }
        
        template<int Index> 
        static bool scan(BitBoard jumper, State& capture, Stack& moves)
        {
                if (jumper & capture.template targets<Index>()) {
                        capture.make(jumper);
                        generate_next<Index>(jumper, capture, moves);
                        capture.undo(jumper);
                        return true;
                } else
                        return false;
        }

        // tag dispatching on king range
        template<int Index> 
        static void king_slide(BitBoard& jumper, BitBoard path)
        {
                king_slide<Index>(
                        jumper, path, 
                        Int2Type<rules::is_long_king_range<Rules>::value>()
                );
        }
        
        // partial specialization for short ranged kings
        template<int Index> 
        static void king_slide(
                BitBoard& jumper, BitBoard /* path */, Int2Type<rules::range_1>
        )
        {
                PushAssign<Board, Index>()(jumper);
        }
        
        // partial specialization for long ranged kings
        template<int Index> 
        static void king_slide(
                BitBoard& jumper, BitBoard path, Int2Type<rules::range_N>
        )
        {
                do PushAssign<Board, Index>()(jumper); while (jumper & path);
        }

        // tag dispatching on king range
        template<int Index> 
        static bool detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
        {
                return detect_dir<Index>(
                        active_kings, opponent_pieces, not_occupied, 
                        Int2Type<rules::is_long_king_range<Rules>::value>()
                );
        }
        
        // partial specialization for short ranged kings
        template<int Index> 
        static bool detect_dir(
                BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::range_1>
        )
        {
                return !bit::is_zero(
                        Push<Board, Index>()(active_kings) & 
                        opponent_pieces & 
                        Pull<Board, Index>()(not_occupied)
                );
        }
        
        // partial specialization for long ranged kings
        template<int Index> 
        static bool detect_dir(
                BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::range_N>
        )
        {
                return !bit::is_zero(
                        Push<Board, Index>()(FloodFill<Board, Index>()(active_kings, not_occupied)) & 
                        opponent_pieces & 
                        Pull<Board, Index>()(not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
