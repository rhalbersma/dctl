#include "KingJumps.h"
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
void GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& capture)
{
        assert(p.to_move() == Color);
        generate_targets(p, capture);
}

// tag dispatching based on whether men can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_targets(const Position<Board>& p, Propagate<Rules, Board>& capture)
{
        generate_targets(p, capture, Int2Type<Variant::is_MenCaptureKings<Rules>::VALUE>());
}

// partial specialization for men that cannot capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_targets(const Position<Board>& p, Propagate<Rules, Board>& capture, Int2Type<false>)
{
        capture.toggle_opponent_kings();
        generate_targets(p, capture, Int2Type<true>());
        capture.toggle_opponent_kings();
}

// partial specialization for men that can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_targets(const Position<Board>& p, Propagate<Rules, Board>& capture, Int2Type<true>)
{
        generate_dirs(p.men(Color), capture);
}

// tag dispatching based on man capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard active_men, Propagate<Rules, Board>& capture)
{
        generate_dirs(active_men, capture, Int2Type<Variant::ManCaptureDirections<Rules>::VALUE>());
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard active_men, Propagate<Rules, Board>& capture, Int2Type<Variant::DIRS_2>)
{
        generate_dir<DirIndex<Board, Color>::LEFT_UP >(active_men, capture);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP>(active_men, capture);
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard active_men, Propagate<Rules, Board>& capture, Int2Type<Variant::DIRS_4>)
{
        generate_dir<DirIndex<Board, Color>::LEFT_UP   >(active_men, capture);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP  >(active_men, capture);
        generate_dir<DirIndex<Board, Color>::LEFT_DOWN >(active_men, capture);
        generate_dir<DirIndex<Board, Color>::RIGHT_DOWN>(active_men, capture);
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard active_men, Propagate<Rules, Board>& capture, Int2Type<Variant::DIRS_8>)
{
        generate_dir<DirIndex<Board, Color>::LEFT_UP   >(active_men, capture);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP  >(active_men, capture);
        generate_dir<DirIndex<Board, Color>::LEFT_DOWN >(active_men, capture);
        generate_dir<DirIndex<Board, Color>::RIGHT_DOWN>(active_men, capture);
        generate_dir<DirIndex<Board, Color>::LEFT      >(active_men, capture);
        generate_dir<DirIndex<Board, Color>::RIGHT     >(active_men, capture);
        generate_dir<DirIndex<Board, Color>::UP        >(active_men, capture);
        generate_dir<DirIndex<Board, Color>::DOWN      >(active_men, capture);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_dir(BitBoard active_men, Propagate<Rules, Board>& capture)
{
        BitBoard jump_sq, target_sq;
        for (active_men &= capture.template man_jumpers<Index>(); active_men; Bit::clear_lowest(active_men)) {
                jump_sq = Bit::get_lowest(active_men);
                target_sq = Shift<DirTraits<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
                capture.launch(jump_sq);
                capture.make(target_sq);
                generate_next<Index>(target_sq, capture);
                capture.undo(target_sq);
                capture.finish(jump_sq);
        }
}

template<bool Color, typename Rules, typename Board> template<size_t Index> 
void GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_next(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
	if (!scan_next<Index>(jump_sq, capture) && capture.current_greater_equal_best()) {
		if (capture.current_not_equal_to_best())
			capture.improve_best();
		capture.template add_man_capture<Color>(jump_sq);
	}
}

// tag dispatching based on promotion condition
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_next<Index>(jump_sq, capture, Int2Type<Variant::PromotionCondition<Rules>::VALUE>());
}

// partial specialization for men that promote on the back row
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<Variant::PROMOTE_BR>)
{
        return scan_dirs<Index>(jump_sq, capture);
}

// partial specialization for men that promote en-passant
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<Variant::PROMOTE_EP>)
{
        if (!capture.is_promotion<Color>(jump_sq))
                return scan_dirs<Index>(jump_sq, capture);
        else {
                capture.toggle_promotion();
                const bool found_next = GenerateDriver<Color, Pieces::KING, Move::JUMPS, Rules, Board>::promotions_en_passant<Index>(jump_sq, capture);
                capture.toggle_promotion();
                return found_next;
        }
}

// tag dispatching based on man capture directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_dirs<Index>(jump_sq, capture, Int2Type<Variant::ManCaptureDirections<Rules>::VALUE>());
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<Variant::DIRS_2>)
{
        return (
                scan_dir<MirrorDirIndex<Index>::U090>(jump_sq, capture) |
                scan_dir<Index>(jump_sq, capture)
        );
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<Variant::DIRS_4>)
{
        return (
                scan_dir<RotateDirIndex<Index, R090>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, L090>::VALUE>(jump_sq, capture) |
                scan_dir<Index>(jump_sq, capture)
        );
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<Variant::DIRS_8>)
{
        return (
                scan_dir<RotateDirIndex<Index, R045>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, L045>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, R090>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, L090>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, R135>::VALUE>(jump_sq, capture) |
                scan_dir<RotateDirIndex<Index, L135>::VALUE>(jump_sq, capture) |
                scan_dir<Index>(jump_sq, capture)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_dir(BitBoard target_sq, Propagate<Rules, Board>& capture)
{
        ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(target_sq, Board::DIR[Index]);
        return scan<Index>(target_sq, capture);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan(BitBoard target_sq, Propagate<Rules, Board>& capture)
{
        if (target_sq & capture.template targets<Index>()) {
                capture.make(target_sq);
                generate_next<Index>(target_sq, capture);
                capture.undo(target_sq);
                return true;
        } else
                return false;
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::count(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        Propagate<Rules, Board> captures(p);
        generate(p, captures);
        return captures.size();
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return detect_targets(p);
}

// tag dispatching based on whether men can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_targets(const Position<Board>& p)
{
        return detect_targets(p, Int2Type<Variant::is_MenCaptureKings<Rules>::VALUE>());
}

// partial specialization for men that cannot capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_targets(const Position<Board>& p, Int2Type<false>)
{
        return detect_dirs(p.men(Color), p.men(!Color), p.not_occupied());
}

// partial specialization for men that can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_targets(const Position<Board>& p, Int2Type<true>)
{
        return detect_dirs(p.men(Color), p.pieces(!Color), p.not_occupied());
}

// tag dispatching based on man capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<Variant::ManCaptureDirections<Rules>::VALUE>());
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variant::DIRS_2>)
{
        return (
                detect_dir<DirIndex<Board, Color>::LEFT_UP >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_UP>(active_men, opponent_pieces, not_occupied)
        );
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variant::DIRS_4>)
{
        return (
                detect_dir<DirIndex<Board, Color>::LEFT_UP   >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_UP  >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::LEFT_DOWN >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_DOWN>(active_men, opponent_pieces, not_occupied)
        );
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variant::DIRS_8>)
{
        return (
                detect_dir<DirIndex<Board, Color>::LEFT_UP   >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_UP  >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::LEFT_DOWN >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_DOWN>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::LEFT      >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT     >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::UP        >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::DOWN      >(active_men, opponent_pieces, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_dir(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return !Bit::is_zero(Shift<DirTraits<Index>::IS_POSITIVE>()(active_men, Board::DIR[Index]) & opponent_pieces & Shift<DirTraits<Index>::IS_NEGATIVE>()(not_occupied, Board::DIR[Index]));
}
