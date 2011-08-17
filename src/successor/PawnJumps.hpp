#include <cassert>
#include "KingJumps.h"
#include "../capture/State.h"
#include "../node/Position.h"
#include "../board/Angle.h"
#include "../board/Board.h"
#include "../board/Direction.h"
#include "../bit/Bit.h"
#include "../utils/Shift.h"

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

// tag dispatching based on whether men can capture kings
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

// tag dispatching based on man capture directions
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
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::DIRS_ALL>
)
{
        generate_dirs(active_men, capture, moves, Int2Type<rules::DIRS_ORTH>());
        generate_dirs(active_men, capture, moves, Int2Type<rules::DIRS_DIAG>());
}

// partial specialization for men that capture in the 4 orthogonal directions
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_dirs(
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::DIRS_ORTH>
)
{
        generate_dir<Direction::LEFT >(active_men, capture, moves);
        generate_dir<Direction::RIGHT>(active_men, capture, moves);
        generate_dir<Direction::UP   >(active_men, capture, moves);
        generate_dir<Direction::DOWN >(active_men, capture, moves);
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_dirs(
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::DIRS_DIAG>
)
{
        generate_dirs(active_men, capture, moves, Int2Type<rules::DIRS_UP  >());
        generate_dirs(active_men, capture, moves, Int2Type<rules::DIRS_DOWN>());
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_dirs(
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::DIRS_UP>
)
{
        generate_dir<Direction::LEFT_UP >(active_men, capture, moves);
        generate_dir<Direction::RIGHT_UP>(active_men, capture, moves);
}

// partial specialization for men that capture in the 2 backward diagonal directions
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Jumps, Rules, Board>::generate_dirs(
        BitBoard active_men, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::DIRS_DOWN>
)
{
        generate_dir<Direction::LEFT_DOWN >(active_men, capture, moves);
        generate_dir<Direction::RIGHT_DOWN>(active_men, capture, moves);
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

// tag dispatching based on promotion condition
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
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::PROMOTE_EP>
)
{
        if (!capture.is_promotion<Color>(jumper))
                return scan_next<Index>(jumper, capture, moves, Int2Type<rules::PROMOTE_BR>());
        else {
                capture.toggle_promotion();
                const bool found_next = Driver<Color, Material::KING, Jumps, Rules, Board>::promote_en_passant<Index>(jumper, capture, moves);
                capture.toggle_promotion();
                return found_next;
        }
}

// partial specialization for men that promote on the back row
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_next(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::PROMOTE_BR>
)
{
        return (
                scan_dirs<Index>(jumper, capture, moves) |
                scan_dir<Index>(jumper, capture, moves)
        );
}

// tag dispatching based on man scan directions
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
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::SCAN_ALL>
)
{
        return (
                scan_dirs<Index>(jumper, capture, moves, Int2Type<rules::SCAN_SIDE>()) |
                scan_dirs<Index>(jumper, capture, moves, Int2Type<rules::SCAN_REST >())
        );
}

// partial specialization for scans in the remaining 4 diagonal or orthogonal directions
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_dirs(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::SCAN_REST>
)
{
        return (
                scan_dir<board::Rotate<Int2Type<Index>, board::Angle::R045>::value>(jumper, capture, moves) |
                scan_dir<board::Rotate<Int2Type<Index>, board::Angle::L045>::value>(jumper, capture, moves) |
                scan_dir<board::Rotate<Int2Type<Index>, board::Angle::R135>::value>(jumper, capture, moves) |
                scan_dir<board::Rotate<Int2Type<Index>, board::Angle::L135>::value>(jumper, capture, moves)
        );
}

// partial specialization for scans in the 2 sideways directions
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_dirs(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::SCAN_SIDE>
)
{
        return (
                scan_dir<board::Rotate<Int2Type<Index>, board::Angle::R090>::value>(jumper, capture, moves) |
                scan_dir<board::Rotate<Int2Type<Index>, board::Angle::L090>::value>(jumper, capture, moves)
        );
}

// partial specialization for scans in the 1 mirrored forward direction
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_dirs(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::SCAN_UP>
)
{
        return scan_dir<board::MirrorUp<Index>::value>(jumper, capture, moves);
}

// partial specialization for scans in the 1 mirrored backward direction
template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::scan_dirs(
        BitBoard jumper, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<rules::SCAN_DOWN>
)
{
        return scan_dir<board::MirrorDown<Index>::value>(jumper, capture, moves);
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

// tag dispatching based on whether men can capture kings
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

// tag dispatching based on man capture directions
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
        BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::DIRS_ALL>
)
{
        return (
                detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::DIRS_ORTH>()) ||
                detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::DIRS_DIAG>())
        );
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_dirs(
        BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::DIRS_ORTH>
)
{
        return (
                detect_dir<Direction::LEFT >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<Direction::RIGHT>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<Direction::UP   >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<Direction::DOWN >(active_men, opponent_pieces, not_occupied)
        );
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_dirs(
        BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::DIRS_DIAG>
)
{
        return (
                detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::DIRS_UP  >()) ||
                detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<rules::DIRS_DOWN>())
        );
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_dirs(
        BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::DIRS_UP>
)
{
        return (
                detect_dir<Direction::LEFT_UP >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<Direction::RIGHT_UP>(active_men, opponent_pieces, not_occupied)
        );
}

// partial specialization for men that capture in the 2 backward diagonal directions
template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Jumps, Rules, Board>::detect_dirs(
        BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<rules::DIRS_DOWN>
)
{
        return (
                detect_dir<Direction::LEFT_DOWN >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<Direction::RIGHT_DOWN>(active_men, opponent_pieces, not_occupied)
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
