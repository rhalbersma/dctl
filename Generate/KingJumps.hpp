#include "Capture/State.h"
#include "../Geometry/Board.h"
#include "../Geometry/Direction.h"
#include "../Geometry/PushPull.h"
#include "../Position/Position.h"
#include "../Utilities/Bit.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

using namespace Geometry::Direction;

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, Move::List& move_list)
{
        Capture::State<Rules, Board> capture(p);
        generate(p, capture, move_list);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        generate_precede(p, capture, move_list);
}

// tag dispatching based on relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        generate_precede(p, capture, move_list, Int2Type<Variants::is_RelativeKingPrecedence<Rules>::VALUE>());
}

// partial specialization for relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<true>)
{
        capture.toggle_with_king();
        generate_precede(p, capture, move_list, Int2Type<false>());
        capture.toggle_with_king();
}

// partial specialization for no relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<false>)
{
        generate_serial(p.kings(Color), capture, move_list);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_serial(BitBoard active_kings, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        assert(!Bit::is_zero(active_kings));
        BitBoard jumper;
        do {
                jumper = Bit::get_lowest(active_kings);
                capture.launch(jumper);
                generate_dirs(jumper, capture, move_list);
                capture.finish(jumper);
                Bit::clear_lowest(active_kings);
        } while (active_kings);
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        generate_dirs(jumper, capture, move_list, Int2Type<Variants::KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<Variants::DIRS_4>)
{
        generate_dir<Indices<Board, Color>::LEFT_UP   >(jumper, capture, move_list);
        generate_dir<Indices<Board, Color>::RIGHT_UP  >(jumper, capture, move_list);
        generate_dir<Indices<Board, Color>::LEFT_DOWN >(jumper, capture, move_list);
        generate_dir<Indices<Board, Color>::RIGHT_DOWN>(jumper, capture, move_list);
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<Variants::DIRS_8>)
{
        generate_dir<Indices<Board, Color>::LEFT_UP   >(jumper, capture, move_list);
        generate_dir<Indices<Board, Color>::RIGHT_UP  >(jumper, capture, move_list);
        generate_dir<Indices<Board, Color>::LEFT_DOWN >(jumper, capture, move_list);
        generate_dir<Indices<Board, Color>::RIGHT_DOWN>(jumper, capture, move_list);
        generate_dir<Indices<Board, Color>::LEFT      >(jumper, capture, move_list);
        generate_dir<Indices<Board, Color>::RIGHT     >(jumper, capture, move_list);
        generate_dir<Indices<Board, Color>::UP        >(jumper, capture, move_list);
        generate_dir<Indices<Board, Color>::DOWN      >(jumper, capture, move_list);
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dir(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        return generate_dir<Index>(jumper, capture, move_list, Int2Type<Variants::is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dir(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<Variants::RANGE_1>)
{
        PushAssign<Board, Index>()(jumper);
        if (jumper & capture.template targets<Index>()) {
                capture.make(jumper);
                generate_next<Index>(jumper, capture, move_list);
                capture.undo(jumper);
        }
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dir(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<Variants::RANGE_N>)
{
        do PushAssign<Board, Index>()(jumper); while (jumper & capture.template path<Index>());
        if (jumper & capture.template targets<Index>()) {
                capture.make(jumper);
                generate_next<Index>(jumper, capture, move_list);
                capture.undo(jumper);
        }
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
void Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_next(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        PushAssign<Board, Index>()(jumper);
        if (!scan_next<Index>(jumper, capture, move_list) && capture.current_greater_equal_best()) {
		if (capture.current_not_equal_to_best())
			capture.improve_best(move_list);
		capture.template add_king_capture<Color, Index>(jumper, move_list);
        }
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_next(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        return scan_next<Index>(jumper, capture, move_list, Int2Type<Variants::is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_next(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<Variants::RANGE_1>)
{
        return (
                scan_dirs<Index>(jumper, capture, move_list) |
                scan_dir<Index>(jumper, capture, move_list)
        );
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_next(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<Variants::RANGE_N>)
{
        return scan_long<Index>(jumper, capture, move_list);
}

// tag dispatching based on king capture direction reversal
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_long(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        return scan_long<Index>(jumper, capture, move_list, Int2Type<Variants::is_DirectionReversal<Rules>::VALUE>());
}

// partial specialization for kings that cannot reverse their capture direction
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_long(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<false>)
{
        return scan_forward<Index>(jumper, capture, move_list);
}

// partial specialization for kings that can reverse their capture direction
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_long(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<true>)
{
        return (
                scan_reverse<Index>(jumper, capture, move_list) |
                scan_forward<Index>(jumper, capture, move_list)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_reverse(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        return scan_dir<Rotate<Index>::I180>(jumper, capture, move_list);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_forward(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        assert(jumper & capture.path());
        bool found_capture = false;
        do {
                found_capture |= scan_dirs<Index>(jumper, capture, move_list);
                PushAssign<Board, Index>()(jumper);
        } while (jumper & capture.path());
        return found_capture |= scan<Index>(jumper, capture, move_list);
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        return scan_dirs<Index>(jumper, capture, move_list, Int2Type<Variants::KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<Variants::DIRS_4>)
{
        return (
                scan_dir<Rotate<Index>::R090>(jumper, capture, move_list) |
                scan_dir<Rotate<Index>::L090>(jumper, capture, move_list)
        );
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<Variants::DIRS_8>)
{
        return (
                scan_dir<Rotate<Index>::R045>(jumper, capture, move_list) |
                scan_dir<Rotate<Index>::L045>(jumper, capture, move_list) |
                scan_dir<Rotate<Index>::R090>(jumper, capture, move_list) |
                scan_dir<Rotate<Index>::L090>(jumper, capture, move_list) |
                scan_dir<Rotate<Index>::R135>(jumper, capture, move_list) |
                scan_dir<Rotate<Index>::L135>(jumper, capture, move_list)
        );
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dir(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        return scan_dir<Index>(jumper, capture, move_list, Int2Type<Variants::is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dir(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<Variants::RANGE_1>)
{
        PushAssign<Board, Index>()(jumper);
        return scan<Index>(jumper, capture, move_list);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dir(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list, Int2Type<Variants::RANGE_N>)
{
        do PushAssign<Board, Index>()(jumper); while (jumper & capture.template path<Index>());
        return scan<Index>(jumper, capture, move_list);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        if (jumper & capture.template targets<Index>()) {
                capture.make(jumper);
                generate_next<Index>(jumper, capture, move_list);
                capture.undo(jumper);
                return true;
        } else
                return false;
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::promote_en_passant(BitBoard jumper, Capture::State<Rules, Board>& capture, Move::List& move_list)
{
        return scan_next<Index>(jumper, capture, move_list);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::count(const Position<Board>& p)
{
        Move::List move_list;
        generate(p, move_list);
        return move_list.size();
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect(const Position<Board>& p)
{
        return detect_dirs(p.kings(Color), p.pieces(!Color), p.not_occupied());
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dirs(active_kings, opponent_pieces, not_occupied, Int2Type<Variants::KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variants::DIRS_4>)
{
        return (
                detect_dir<Indices<Board, Color>::LEFT_UP   >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_UP  >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::LEFT_DOWN >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_DOWN>(active_kings, opponent_pieces, not_occupied)
        );
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variants::DIRS_8>)
{
        return (
                detect_dir<Indices<Board, Color>::LEFT_UP   >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_UP  >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::LEFT_DOWN >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_DOWN>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::LEFT      >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT     >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::UP        >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<Indices<Board, Color>::DOWN      >(active_kings, opponent_pieces, not_occupied)
        );
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dir<Index>(active_kings, opponent_pieces, not_occupied, Int2Type<Variants::is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variants::RANGE_1>)
{
        return !Bit::is_zero(Push<Board, Index>()(active_kings) & opponent_pieces & Pull<Board, Index>()(not_occupied));
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variants::RANGE_N>)
{
        return !Bit::is_zero(Push<Board, Index>()(FloodFill<Board, Index>()(active_kings, not_occupied)) & opponent_pieces & Pull<Board, Index>()(not_occupied));
}
