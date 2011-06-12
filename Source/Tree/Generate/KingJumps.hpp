#include <cassert>
#include "Capture/State.h"
#include "../Node/Position.h"
#include "../../Geometry/Board.h"
#include "../../Geometry/Direction.h"
#include "../../Geometry/PushPull.h"
#include "../../Utilities/Bit.h"

using namespace geometry::direction;

namespace tree {
namespace generate {

template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate(const node::Position<Board>& p, move::Stack* move_stack)
{
        capture::State<Rules, Board> capture(p);
        generate(p, capture, move_stack);
}

template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate(const node::Position<Board>& p, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        generate_precede(p, capture, move_stack);
}

// tag dispatching based on relative king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate_precede(const node::Position<Board>& p, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        generate_precede(p, capture, move_stack, Int2Type<variants::is_relative_king_precedence<Rules>::value>());
}

// partial specialization for relative king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate_precede(const node::Position<Board>& p, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<true>)
{
        capture.toggle_with_king();
        generate_precede(p, capture, move_stack, Int2Type<false>());
        capture.toggle_with_king();
}

// partial specialization for no relative king capture precedence
template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate_precede(const node::Position<Board>& p, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<false>)
{
        generate_serial(p.kings(Color), capture, move_stack);
}

template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate_serial(BitBoard active_kings, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        assert(!bit::is_zero(active_kings));
        BitBoard jumper;
        do {
                jumper = bit::get_lowest(active_kings);
                capture.launch(jumper);
                generate_dirs(jumper, capture, move_stack);
                capture.finish(jumper);
                bit::clear_lowest(active_kings);
        } while (active_kings);
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate_dirs(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        generate_dirs(jumper, capture, move_stack, Int2Type<variants::king_capture_directions<Rules>::value>());
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate_dirs(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<variants::DIRS_ALL>)
{
        generate_dirs(jumper, capture, move_stack, Int2Type<variants::DIRS_ORTH>());
        generate_dirs(jumper, capture, move_stack, Int2Type<variants::DIRS_DIAG>());
}

// partial specialization for kings that capture in the 4 orthogonal directions
template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate_dirs(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<variants::DIRS_ORTH>)
{
        generate_dir<Indices<Board, Color>::LEFT >(jumper, capture, move_stack);
        generate_dir<Indices<Board, Color>::RIGHT>(jumper, capture, move_stack);
        generate_dir<Indices<Board, Color>::UP   >(jumper, capture, move_stack);
        generate_dir<Indices<Board, Color>::DOWN >(jumper, capture, move_stack);
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate_dirs(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<variants::DIRS_DIAG>)
{
        generate_dir<Indices<Board, Color>::LEFT_UP   >(jumper, capture, move_stack);
        generate_dir<Indices<Board, Color>::RIGHT_UP  >(jumper, capture, move_stack);
        generate_dir<Indices<Board, Color>::LEFT_DOWN >(jumper, capture, move_stack);
        generate_dir<Indices<Board, Color>::RIGHT_DOWN>(jumper, capture, move_stack);
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate_dir(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        king_slide<Index>(jumper, capture.template path<Index>());
        if (jumper & capture.template targets<Index>()) {
                capture.make(jumper);
                generate_next<Index>(jumper, capture, move_stack);
                capture.undo(jumper);
        }
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::generate_next(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        PushAssign<Board, Index>()(jumper);
        if (!scan_next<Index>(jumper, capture, move_stack) && capture.current_greater_equal_best()) {
		if (capture.current_not_equal_to_best())
			capture.improve_best(move_stack);
		capture.template add_king_capture<Color, Index>(jumper, move_stack);
        }
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::promote_en_passant(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        return scan_next<Index>(jumper, capture, move_stack);
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_next(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        return scan_next<Index>(jumper, capture, move_stack, Int2Type<variants::is_long_king_range<Rules>::value>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_next(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<variants::RANGE_1>)
{
        return (
                scan_dirs<Index>(jumper, capture, move_stack) |
                scan_dir<Index>(jumper, capture, move_stack)
        );
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_next(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<variants::RANGE_N>)
{
        return scan_long<Index>(jumper, capture, move_stack);
}

// tag dispatching based on king capture direction reversal
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_long(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        return scan_long<Index>(jumper, capture, move_stack, Int2Type<variants::is_capture_direction_reversal<Rules>::value>());
}

// partial specialization for kings that cannot reverse their capture direction
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_long(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<false>)
{
        return scan_forward<Index>(jumper, capture, move_stack);
}

// partial specialization for kings that can reverse their capture direction
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_long(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<true>)
{
        return (
                scan_reverse<Index>(jumper, capture, move_stack) |
                scan_forward<Index>(jumper, capture, move_stack)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_reverse(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        return scan_dir<Rotate<Index>::I180>(jumper, capture, move_stack);
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_forward(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        assert(jumper & capture.path());
        bool found_capture = false;
        do {
                found_capture |= scan_dirs<Index>(jumper, capture, move_stack);
                PushAssign<Board, Index>()(jumper);
        } while (jumper & capture.path());
        return found_capture |= scan<Index>(jumper, capture, move_stack);
}

// tag dispatching based on king scan directions
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_dirs(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        return scan_dirs<Index>(jumper, capture, move_stack, Int2Type<variants::king_scan_directions<Rules>::value>());
}

// partial specialization for scans in all the 6 non-parallel diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_dirs(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<variants::SCAN_ALL>)
{
        return (
                scan_dirs<Index>(jumper, capture, move_stack, Int2Type<variants::SCAN_REST>()) |
                scan_dirs<Index>(jumper, capture, move_stack, Int2Type<variants::SCAN_SIDE>())
        );
}

// partial specialization for scans in the remaining 4 diagonal or orthogonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_dirs(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<variants::SCAN_REST>)
{
        return (
                scan_dir<Rotate<Index>::R045>(jumper, capture, move_stack) |
                scan_dir<Rotate<Index>::L045>(jumper, capture, move_stack) |
                scan_dir<Rotate<Index>::R135>(jumper, capture, move_stack) |
                scan_dir<Rotate<Index>::L135>(jumper, capture, move_stack)
        );
}

// partial specialization for scans in the 2 sideways directions
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_dirs(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack, Int2Type<variants::SCAN_SIDE>)
{
        return (
                scan_dir<Rotate<Index>::R090>(jumper, capture, move_stack) |
                scan_dir<Rotate<Index>::L090>(jumper, capture, move_stack)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan_dir(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        king_slide<Index>(jumper, capture.template path<Index>());
        return scan<Index>(jumper, capture, move_stack);
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::scan(BitBoard jumper, capture::State<Rules, Board>& capture, move::Stack* move_stack)
{
        if (jumper & capture.template targets<Index>()) {
                capture.make(jumper);
                generate_next<Index>(jumper, capture, move_stack);
                capture.undo(jumper);
                return true;
        } else
                return false;
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::king_slide(BitBoard& jumper, BitBoard path)
{
        king_slide<Index>(jumper, path, Int2Type<variants::is_long_king_range<Rules>::value>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::king_slide(BitBoard& jumper, BitBoard, Int2Type<variants::RANGE_1>)
{
        PushAssign<Board, Index>()(jumper);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
void Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::king_slide(BitBoard& jumper, BitBoard path, Int2Type<variants::RANGE_N>)
{
        do PushAssign<Board, Index>()(jumper); while (jumper & path);
}

template<bool Color, typename Rules, typename Board>
size_t Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::count(const node::Position<Board>& p)
{
        move::Stack move_stack;
        generate(p, &move_stack);
        return move_stack.size();
}

template<bool Color, typename Rules, typename Board>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::detect(const node::Position<Board>& p)
{
        return detect_dirs(p.kings(Color), p.pieces(!Color), p.not_occupied());
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dirs(active_kings, opponent_pieces, not_occupied, Int2Type<variants::king_capture_directions<Rules>::value>());
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<variants::DIRS_ALL>)
{
        return (
                detect_dirs(active_kings, opponent_pieces, not_occupied, Int2Type<variants::DIRS_ORTH>()) ||
                detect_dirs(active_kings, opponent_pieces, not_occupied, Int2Type<variants::DIRS_DIAG>())
        );
}

// partial specialization for kings that capture in the 4 orthogonal directions
template<bool Color, typename Rules, typename Board>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<variants::DIRS_ORTH>)
{
        return (
                detect_dir<Indices<Board, Color>::LEFT >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::UP   >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::DOWN >(active_kings, opponent_pieces, not_occupied)
        );
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<variants::DIRS_DIAG>)
{
        return (
                detect_dir<Indices<Board, Color>::LEFT_UP   >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_UP  >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::LEFT_DOWN >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_DOWN>(active_kings, opponent_pieces, not_occupied)
        );
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dir<Index>(active_kings, opponent_pieces, not_occupied, Int2Type<variants::is_long_king_range<Rules>::value>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<variants::RANGE_1>)
{
        return !bit::is_zero(Push<Board, Index>()(active_kings) & opponent_pieces & Pull<Board, Index>()(not_occupied));
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Template<Color, node::Pieces::KING, move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<variants::RANGE_N>)
{
        return !bit::is_zero(Push<Board, Index>()(FloodFill<Board, Index>()(active_kings, not_occupied)) & opponent_pieces & Pull<Board, Index>()(not_occupied));
}

}       // namespace generate
}       // namespace tree
