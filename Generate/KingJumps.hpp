#include "Propagate.h"
#include "../Board/Board.h"
#include "../Board/Direction.h"
#include "../Board/Transform.h"
#include "../Position/Position.h"
#include "../Utilities/Bit.h"
#include "../Utilities/Shift.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& capture)
{
        assert(p.to_move() == Color);
        generate_precede(p, capture);
}

// tag dispatching based on relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture)
{
        generate_precede(p, capture, Int2Type<is_RelativeKingPrecedence<Rules>::VALUE>());
}

// partial specialization for relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture, Int2Type<true>)
{
        capture.toggle_with_king();
        generate_precede(p, capture, Int2Type<false>());
        capture.toggle_with_king();
}

// partial specialization for no relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture, Int2Type<false>)
{
        generate_serial(p.kings(Color), capture);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_serial(BitBoard active_kings, Propagate<Rules, Board>& capture)
{
        assert(!Bit::is_zero(active_kings));
        BitBoard jump_sq;
        do {
                jump_sq = Bit::get_lowest(active_kings);
                capture.launch(jump_sq);
                generate_dirs(jump_sq, capture);
                capture.finish(jump_sq);
                Bit::clear_lowest(active_kings);
        } while (active_kings);
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        generate_dirs(jump_sq, capture, Int2Type<KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<DIRS_4>)
{
        generate_dir<DirIndex<Board, Color>::LEFT_UP   >(jump_sq, capture);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP  >(jump_sq, capture);
        generate_dir<DirIndex<Board, Color>::LEFT_DOWN >(jump_sq, capture);
        generate_dir<DirIndex<Board, Color>::RIGHT_DOWN>(jump_sq, capture);
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<DIRS_8>)
{
        generate_dir<DirIndex<Board, Color>::LEFT_UP   >(jump_sq, capture);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP  >(jump_sq, capture);
        generate_dir<DirIndex<Board, Color>::LEFT_DOWN >(jump_sq, capture);
        generate_dir<DirIndex<Board, Color>::RIGHT_DOWN>(jump_sq, capture);
        generate_dir<DirIndex<Board, Color>::LEFT      >(jump_sq, capture);
        generate_dir<DirIndex<Board, Color>::RIGHT     >(jump_sq, capture);
        generate_dir<DirIndex<Board, Color>::UP        >(jump_sq, capture);
        generate_dir<DirIndex<Board, Color>::DOWN      >(jump_sq, capture);
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return generate_dir<Index>(jump_sq, capture, Int2Type<is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_1>)
{
        ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
        if (jump_sq & capture.template targets<Index>()) {
                capture.make(jump_sq);
                generate_next<Index>(jump_sq, capture);
                capture.undo(jump_sq);
        }
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_N>)
{
        do ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]); while (jump_sq & capture.template path<Index>());
        if (jump_sq & capture.template targets<Index>()) {
                capture.make(jump_sq);
                generate_next<Index>(jump_sq, capture);
                capture.undo(jump_sq);
        }
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
void GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::generate_next(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
        if (!scan_next<Index>(jump_sq, capture) && capture.current_greater_equal_best()) {
		if (capture.current_not_equal_to_best())
			capture.improve_best();
		capture.template add_king_capture<Color, Index>(jump_sq);
        }
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_next<Index>(jump_sq, capture, Int2Type<is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_1>)
{
        return (
                scan_dirs<Index>(jump_sq, capture) |
                scan_dir<Index>(jump_sq, capture)
        );
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_N>)
{
        return scan_long<Index>(jump_sq, capture);
}

// tag dispatching based on king capture direction reversal
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_long(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_long<Index>(jump_sq, capture, Int2Type<is_DirectionReversal<Rules>::VALUE>());
}

// partial specialization for kings that cannot reverse their capture direction
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_long(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<false>)
{
        return scan_forward<Index>(jump_sq, capture);
}

// partial specialization for kings that can reverse their capture direction
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_long(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<true>)
{
        return (
                scan_reverse<Index>(jump_sq, capture) |
                scan_forward<Index>(jump_sq, capture)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_reverse(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_dir<RotateDirIndex<Index, D180>::VALUE>(jump_sq, capture);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_forward(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        assert(jump_sq & capture.path());
        bool found_capture = false;
        do {
                found_capture |= scan_dirs<Index>(jump_sq, capture);
                ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
        } while (jump_sq & capture.path());
        return found_capture |= scan<Index>(jump_sq, capture);
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_dirs<Index>(jump_sq, capture, Int2Type<KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<DIRS_4>)
{
        return (
                scan_dir<RotateDirIndex<Index, R090>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, L090>::VALUE>(jump_sq, capture)
        );
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<DIRS_8>)
{
        return (
                scan_dir<RotateDirIndex<Index, R045>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, L045>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, R090>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, L090>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, R135>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, L135>::VALUE>(jump_sq, capture)
        );
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_dir<Index>(jump_sq, capture, Int2Type<is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_1>)
{
        ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
        return scan<Index>(jump_sq, capture);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_N>)
{
        do ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]); while (jump_sq & capture.template path<Index>());
        return scan<Index>(jump_sq, capture);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::scan(BitBoard target_sq, Propagate<Rules, Board>& capture)
{
        if (target_sq & capture.template targets<Index>()) {
                capture.make(target_sq);
                generate_next<Index>(target_sq, capture);
                capture.undo(target_sq);
                return true;
        } else
                return false;
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::promotions_en_passant(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_next<Index>(jump_sq, capture);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::count(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        Propagate<Rules, Board> captures(p);
        generate(p, captures);
        return captures.size();
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return detect_dirs(p.kings(Color), p.pieces(!Color), p.not_occupied());
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dirs(active_kings, opponent_pieces, not_occupied, Int2Type<KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<DIRS_4>)
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
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<DIRS_8>)
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
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dir<Index>(active_kings, opponent_pieces, not_occupied, Int2Type<is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<RANGE_1>)
{
        return !Bit::is_zero(Shift<DirTraits<Index>::IS_POSITIVE>()(active_kings, Board::DIR[Index]) & opponent_pieces & Shift<DirTraits<Index>::IS_NEGATIVE>()(not_occupied, Board::DIR[Index]));
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<RANGE_N>)
{
        return !Bit::is_zero(Shift<DirTraits<Index>::IS_POSITIVE>()(Bit::flood_fill<DirTraits<Index>::IS_POSITIVE>(active_kings, not_occupied, Board::DIR[Index]), Board::DIR[Index]) & opponent_pieces & Shift<DirTraits<Index>::IS_NEGATIVE>()(not_occupied, Board::DIR[Index]));
}
