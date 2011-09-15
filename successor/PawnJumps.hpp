#include <boost/assert.hpp>
#include "../capture/State.h"
#include "../node/Position.h"
#include "../board/Angle.hpp"
#include "../board/Board.h"
#include "../board/Direction.hpp"
#include "../bit/Bit.h"
#include "../utility/Shift.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate(
        const Position<Board>& p, Stack& moves
)
{
        capture::State<Rules, Board> capture(p);
        generate(p, capture, moves);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate(
        const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves
)
{
        generate_targets(p, capture, moves);
}

// tag dispatching on whether men can capture kings
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_targets(
        const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves
)
{
        generate_targets(p, capture, moves, Int2Type<rules::is_men_capture_kings<Rules>::value>());
}

// partial specialization for men that cannot capture kings
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_targets(
        const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<false>
)
{
        capture.toggle_king_targets();
        generate_targets(p, capture, moves, Int2Type<true>());
        capture.toggle_king_targets();
}

// partial specialization for men that can capture kings
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_targets(
        const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<true>
)
{
        generate_dirs(p.men(Color), capture, moves);
}

// tag dispatching on man capture directions
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_dirs(
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves
)
{
        generate_dirs(active_men, capture, moves, Int2Type<rules::man_capture_directions<Rules>::value>());
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_dirs(
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::dirs_all>
)
{
        generate_dirs(active_men, capture, moves, Int2Type<rules::dirs_orth>());
        generate_dirs(active_men, capture, moves, Int2Type<rules::dirs_diag>());
}

// partial specialization for men that capture in the 4 orthogonal directions
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_dirs(
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::dirs_orth>
)
{
        generate_dir<Direction::left >(active_men, capture, moves);
        generate_dir<Direction::right>(active_men, capture, moves);
        generate_dir<Direction::up   >(active_men, capture, moves);
        generate_dir<Direction::down >(active_men, capture, moves);
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_dirs(
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::dirs_diag>
)
{
        generate_dirs(active_men, capture, moves, Int2Type<rules::dirs_up  >());
        generate_dirs(active_men, capture, moves, Int2Type<rules::dirs_down>());
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_dirs(
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::dirs_up>
)
{
        generate_dir<Direction::left_up >(active_men, capture, moves);
        generate_dir<Direction::right_up>(active_men, capture, moves);
}

// partial specialization for men that capture in the 2 backward diagonal directions
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_dirs(
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::dirs_down>
)
{
        generate_dir<Direction::left_down >(active_men, capture, moves);
        generate_dir<Direction::right_down>(active_men, capture, moves);
}

template<bool Color, typename Rules, typename Board> template<int Index>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_dir(
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves
)
{
        BitBoard jumper, target;
        for (active_men &= Pull<Board, Index>()(capture.template targets<Index>()); active_men; bit::clear_first(active_men)) {
                jumper = bit::get_first(active_men);
                capture.launch(jumper);
                target = Push<Board, Index>()(jumper);
                capture.make(target);
                generate_next<Index>(target, capture, moves);
                capture.undo(target);
                capture.finish(jumper);
        }
}

template<bool Color, typename Rules, typename Board> template<int Index> 
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_next(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves
)
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
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_next(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves
)
{
        return scan_next<Index>(jumper, capture, moves, Int2Type<rules::promotion_condition<Rules>::value>());
}

// partial specialization for men that promote en-passant
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_next(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::promote_ep>
)
{
        if (!capture.is_promotion<Color>(jumper))
                return scan_next<Index>(jumper, capture, moves, Int2Type<rules::promote_af>());
        else {
                capture.toggle_promotion();
                const bool found_next = KingJumps::promote_en_passant<Index>(jumper, capture, moves);
                capture.toggle_promotion();
                return found_next;
        }
}

// partial specialization for men that promote on the back row
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_next(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::promote_af>
)
{
        return (
                scan_dirs<Index>(jumper, capture, moves) |
                scan_dir<Index>(jumper, capture, moves)
        );
}

// tag dispatching on man scan directions
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_dirs(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves
)
{
        return scan_dirs<Index>(jumper, capture, moves, Int2Type<rules::man_scan_directions<Rules>::value>());
}

// partial specialization for scans in all the 6 non-parallel diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_dirs(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::scan_all>
)
{
        return (
                scan_dirs<Index>(jumper, capture, moves, Int2Type<rules::scan_diag>()) |
                scan_dirs<Index>(jumper, capture, moves, Int2Type<rules::scan_orth >())
        );
}

// partial specialization for scans in the remaining 4 diagonal or orthogonal directions
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_dirs(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::scan_orth>
)
{
        return (
                scan_dir<rotate<Angle<Index>, Degrees::R045>::type::index>(jumper, capture, moves) |
                scan_dir<rotate<Angle<Index>, Degrees::L045>::type::index>(jumper, capture, moves) |
                scan_dir<rotate<Angle<Index>, Degrees::R135>::type::index>(jumper, capture, moves) |
                scan_dir<rotate<Angle<Index>, Degrees::L135>::type::index>(jumper, capture, moves)
        );
}

// partial specialization for scans in the 2 sideways directions
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_dirs(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::scan_diag>
)
{
        return (
                scan_dir<rotate<Angle<Index>, Degrees::R090>::type::index>(jumper, capture, moves) |
                scan_dir<rotate<Angle<Index>, Degrees::L090>::type::index>(jumper, capture, moves)
        );
}

// partial specialization for scans in the 1 mirrored forward direction
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_dirs(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::scan_up>
)
{
        return scan_dir<mirror_up<Index>::value>(jumper, capture, moves);
}

// partial specialization for scans in the 1 mirrored backward direction
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_dirs(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::scan_down>
)
{
        return scan_dir<mirror_down<Index>::value>(jumper, capture, moves);
}

template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_dir(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves
)
{
        PushAssign<Board, Index>()(jumper);
        return scan<Index>(jumper, capture, moves);
}

template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves
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

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::PAWN, Jumps, Rules, Board>::count(
        const Position<Board>& p
)
{
        Stack moves;
        generate(p, moves);
        return static_cast<int>(moves.size());
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect(
        const Position<Board>& p
)
{
        return detect_targets(p);
}

// tag dispatching on whether men can capture kings
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_targets(
        const Position<Board>& p
)
{
        return detect_targets(p, Int2Type<rules::is_men_capture_kings<Rules>::value>());
}

// partial specialization for men that cannot capture kings
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_targets(
        const Position<Board>& p, Int2Type<false>
)
{
        return detect_dirs(p.men(Color), p.men(!Color), not_occupied(p));
}

// partial specialization for men that can capture kings
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_targets(
        const Position<Board>& p, Int2Type<true>
)
{
        return detect_dirs(p.men(Color), p.pieces(!Color), not_occupied(p));
}

// tag dispatching on man capture directions
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_dirs(
        BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied
)
{
        return detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::man_capture_directions<Rules>::value>());
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_dirs(
        BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_all>
)
{
        return (
                detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::dirs_orth>()) ||
                detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::dirs_diag>())
        );
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_dirs(
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
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_dirs(
        BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_diag>
)
{
        return (
                detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::dirs_up  >()) ||
                detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::dirs_down>())
        );
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_dirs(
        BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_up>
)
{
        return (
                detect_dir<Direction::left_up >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<Direction::right_up>(active_men, opponent_pieces, not_occupied)
        );
}

// partial specialization for men that capture in the 2 backward diagonal directions
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_dirs(
        BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::dirs_down>
)
{
        return (
                detect_dir<Direction::left_down >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<Direction::right_down>(active_men, opponent_pieces, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_dir(
        BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied
)
{
        return !bit::is_zero(
                Push<Board, Index>()(active_men) & 
                opponent_pieces & 
                Pull<Board, Index>()(not_occupied)
        );
}

}       // namespace successor
}       // namespace dctl
