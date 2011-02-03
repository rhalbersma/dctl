#include "Propagate.h"
#include "../Board/Board.h"
#include "../Board/Direction.h"
#include "../Board/Transform.h"
#include "../Position/Position.h"
#include "../Utilities/Bit.h"
#include "../Utilities/PushPull.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, MoveList& moves)
{
        Propagate<Rules, Board> capture(p);
        generate(p, capture, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& capture, MoveList& moves)
{
        generate_precede(p, capture, moves);
}

// tag dispatching based on relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture, MoveList& moves)
{
        generate_precede(p, capture, moves, Int2Type<Variant::is_RelativeKingPrecedence<Rules>::VALUE>());
}

// partial specialization for relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<true>)
{
        capture.toggle_with_king();
        generate_precede(p, capture, moves, Int2Type<false>());
        capture.toggle_with_king();
}

// partial specialization for no relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<false>)
{
        generate_serial(p.kings(Color), capture, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_serial(BitBoard active_kings, Propagate<Rules, Board>& capture, MoveList& moves)
{
        assert(!Bit::is_zero(active_kings));
        BitBoard jump_sq;
        do {
                jump_sq = Bit::get_lowest(active_kings);
                capture.launch(jump_sq);
                generate_dirs(jump_sq, capture, moves);
                capture.finish(jump_sq);
                Bit::clear_lowest(active_kings);
        } while (active_kings);
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves)
{
        generate_dirs(jump_sq, capture, moves, Int2Type<Variant::KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<Variant::DIRS_4>)
{
        generate_dir<DirIndex<Board, Color>::LEFT_UP   >(jump_sq, capture, moves);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP  >(jump_sq, capture, moves);
        generate_dir<DirIndex<Board, Color>::LEFT_DOWN >(jump_sq, capture, moves);
        generate_dir<DirIndex<Board, Color>::RIGHT_DOWN>(jump_sq, capture, moves);
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<Variant::DIRS_8>)
{
        generate_dir<DirIndex<Board, Color>::LEFT_UP   >(jump_sq, capture, moves);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP  >(jump_sq, capture, moves);
        generate_dir<DirIndex<Board, Color>::LEFT_DOWN >(jump_sq, capture, moves);
        generate_dir<DirIndex<Board, Color>::RIGHT_DOWN>(jump_sq, capture, moves);
        generate_dir<DirIndex<Board, Color>::LEFT      >(jump_sq, capture, moves);
        generate_dir<DirIndex<Board, Color>::RIGHT     >(jump_sq, capture, moves);
        generate_dir<DirIndex<Board, Color>::UP        >(jump_sq, capture, moves);
        generate_dir<DirIndex<Board, Color>::DOWN      >(jump_sq, capture, moves);
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves)
{
        return generate_dir<Index>(jump_sq, capture, moves, Int2Type<Variant::is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<Variant::RANGE_1>)
{
        PushAssign<Board, Index>()(jump_sq);
        if (jump_sq & capture.template targets<Index>()) {
                capture.make(jump_sq);
                generate_next<Index>(jump_sq, capture, moves);
                capture.undo(jump_sq);
        }
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<Variant::RANGE_N>)
{
        do PushAssign<Board, Index>()(jump_sq); while (jump_sq & capture.template path<Index>());
        if (jump_sq & capture.template targets<Index>()) {
                capture.make(jump_sq);
                generate_next<Index>(jump_sq, capture, moves);
                capture.undo(jump_sq);
        }
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
void GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves)
{
        PushAssign<Board, Index>()(jump_sq);
        if (!scan_next<Index>(jump_sq, capture, moves) && capture.current_greater_equal_best()) {
		if (capture.current_not_equal_to_best())
			capture.improve_best(moves);
		capture.template add_king_capture<Color, Index>(moves, jump_sq);
        }
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves)
{
        return scan_next<Index>(jump_sq, capture, moves, Int2Type<Variant::is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<Variant::RANGE_1>)
{
        return (
                scan_dirs<Index>(jump_sq, capture, moves) |
                scan_dir<Index>(jump_sq, capture, moves)
        );
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<Variant::RANGE_N>)
{
        return scan_long<Index>(jump_sq, capture, moves);
}

// tag dispatching based on king capture direction reversal
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_long(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves)
{
        return scan_long<Index>(jump_sq, capture, moves, Int2Type<Variant::is_DirectionReversal<Rules>::VALUE>());
}

// partial specialization for kings that cannot reverse their capture direction
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_long(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<false>)
{
        return scan_forward<Index>(jump_sq, capture, moves);
}

// partial specialization for kings that can reverse their capture direction
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_long(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<true>)
{
        return (
                scan_reverse<Index>(jump_sq, capture, moves) |
                scan_forward<Index>(jump_sq, capture, moves)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_reverse(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves)
{
        return scan_dir<RotateDirIndex<Index, D180>::VALUE>(jump_sq, capture, moves);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_forward(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves)
{
        assert(jump_sq & capture.path());
        bool found_capture = false;
        do {
                found_capture |= scan_dirs<Index>(jump_sq, capture, moves);
                PushAssign<Board, Index>()(jump_sq);
        } while (jump_sq & capture.path());
        return found_capture |= scan<Index>(jump_sq, capture, moves);
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves)
{
        return scan_dirs<Index>(jump_sq, capture, moves, Int2Type<Variant::KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<Variant::DIRS_4>)
{
        return (
                scan_dir<RotateDirIndex<Index, R090>::VALUE>(jump_sq, capture, moves) |
                scan_dir<RotateDirIndex<Index, L090>::VALUE>(jump_sq, capture, moves)
        );
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<Variant::DIRS_8>)
{
        return (
                scan_dir<RotateDirIndex<Index, R045>::VALUE>(jump_sq, capture, moves) |
                scan_dir<RotateDirIndex<Index, L045>::VALUE>(jump_sq, capture, moves) |
                scan_dir<RotateDirIndex<Index, R090>::VALUE>(jump_sq, capture, moves) |
                scan_dir<RotateDirIndex<Index, L090>::VALUE>(jump_sq, capture, moves) |
                scan_dir<RotateDirIndex<Index, R135>::VALUE>(jump_sq, capture, moves) |
                scan_dir<RotateDirIndex<Index, L135>::VALUE>(jump_sq, capture, moves)
        );
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves)
{
        return scan_dir<Index>(jump_sq, capture, moves, Int2Type<Variant::is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<Variant::RANGE_1>)
{
        PushAssign<Board, Index>()(jump_sq);
        return scan<Index>(jump_sq, capture, moves);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves, Int2Type<Variant::RANGE_N>)
{
        do PushAssign<Board, Index>()(jump_sq); while (jump_sq & capture.template path<Index>());
        return scan<Index>(jump_sq, capture, moves);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan(BitBoard target_sq, Propagate<Rules, Board>& capture, MoveList& moves)
{
        if (target_sq & capture.template targets<Index>()) {
                capture.make(target_sq);
                generate_next<Index>(target_sq, capture, moves);
                capture.undo(target_sq);
                return true;
        } else
                return false;
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::promote_en_passant(BitBoard jump_sq, Propagate<Rules, Board>& capture, MoveList& moves)
{
        return scan_next<Index>(jump_sq, capture, moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::count(const Position<Board>& p)
{
        MoveList moves;
        generate(p, moves);
        return moves.size();
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect(const Position<Board>& p)
{
        return detect_dirs(p.kings(Color), p.pieces(!Color), p.not_occupied());
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dirs(active_kings, opponent_pieces, not_occupied, Int2Type<Variant::KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variant::DIRS_4>)
{
        return (
                detect_dir<DirIndex<Board, Color>::LEFT_UP   >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_UP  >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::LEFT_DOWN >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_DOWN>(active_kings, opponent_pieces, not_occupied)
        );
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variant::DIRS_8>)
{
        return (
                detect_dir<DirIndex<Board, Color>::LEFT_UP   >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_UP  >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::LEFT_DOWN >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_DOWN>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::LEFT      >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT     >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::UP        >(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::DOWN      >(active_kings, opponent_pieces, not_occupied)
        );
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dir<Index>(active_kings, opponent_pieces, not_occupied, Int2Type<Variant::is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variant::RANGE_1>)
{
        return !Bit::is_zero(Push<Board, Index>()(active_kings) & opponent_pieces & Pull<Board, Index>()(not_occupied));
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variant::RANGE_N>)
{
        return !Bit::is_zero(Push<Board, Index>()(FloodFill<Board, Index>()(active_kings, not_occupied)) & opponent_pieces & Pull<Board, Index>()(not_occupied));
}
