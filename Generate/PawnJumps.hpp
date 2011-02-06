#include "KingJumps.h"
#include "Capture.h"
#include "../Board/Board.h"
#include "../Board/Direction.h"
#include "../Board/Transform.h"
#include "../Position/Position.h"
#include "../Utilities/Bit.h"
#include "../Utilities/PushPull.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, MoveList& move_list)
{
        Capture<Rules, Board> capture(p);
        generate(p, capture, move_list);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate(const Position<Board>& p, Capture<Rules, Board>& capture, MoveList& move_list)
{
        generate_targets(p, capture, move_list);
}

// tag dispatching based on whether men can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_targets(const Position<Board>& p, Capture<Rules, Board>& capture, MoveList& move_list)
{
        generate_targets(p, capture, move_list, Int2Type<Variant::is_MenCaptureKings<Rules>::VALUE>());
}

// partial specialization for men that cannot capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_targets(const Position<Board>& p, Capture<Rules, Board>& capture, MoveList& move_list, Int2Type<false>)
{
        capture.toggle_king_targets();
        generate_targets(p, capture, move_list, Int2Type<true>());
        capture.toggle_king_targets();
}

// partial specialization for men that can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_targets(const Position<Board>& p, Capture<Rules, Board>& capture, MoveList& move_list, Int2Type<true>)
{
        generate_dirs(p.men(Color), capture, move_list);
}

// tag dispatching based on man capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard active_men, Capture<Rules, Board>& capture, MoveList& move_list)
{
        generate_dirs(active_men, capture, move_list, Int2Type<Variant::ManCaptureDirections<Rules>::VALUE>());
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard active_men, Capture<Rules, Board>& capture, MoveList& move_list, Int2Type<Variant::DIRS_2>)
{
        generate_dir<DirIndex<Board, Color>::LEFT_UP >(active_men, capture, move_list);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP>(active_men, capture, move_list);
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard active_men, Capture<Rules, Board>& capture, MoveList& move_list, Int2Type<Variant::DIRS_4>)
{
        generate_dir<DirIndex<Board, Color>::LEFT_UP   >(active_men, capture, move_list);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP  >(active_men, capture, move_list);
        generate_dir<DirIndex<Board, Color>::LEFT_DOWN >(active_men, capture, move_list);
        generate_dir<DirIndex<Board, Color>::RIGHT_DOWN>(active_men, capture, move_list);
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_dirs(BitBoard active_men, Capture<Rules, Board>& capture, MoveList& move_list, Int2Type<Variant::DIRS_8>)
{
        generate_dir<DirIndex<Board, Color>::LEFT_UP   >(active_men, capture, move_list);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP  >(active_men, capture, move_list);
        generate_dir<DirIndex<Board, Color>::LEFT_DOWN >(active_men, capture, move_list);
        generate_dir<DirIndex<Board, Color>::RIGHT_DOWN>(active_men, capture, move_list);
        generate_dir<DirIndex<Board, Color>::LEFT      >(active_men, capture, move_list);
        generate_dir<DirIndex<Board, Color>::RIGHT     >(active_men, capture, move_list);
        generate_dir<DirIndex<Board, Color>::UP        >(active_men, capture, move_list);
        generate_dir<DirIndex<Board, Color>::DOWN      >(active_men, capture, move_list);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_dir(BitBoard active_men, Capture<Rules, Board>& capture, MoveList& move_list)
{
        BitBoard jumper, target;
        for (active_men &= Pull<Board, Index>()(capture.template targets<Index>()); active_men; Bit::clear_lowest(active_men)) {
                jumper = Bit::get_lowest(active_men);
                capture.launch(jumper);
                target = Push<Board, Index>()(jumper);
                capture.make(target);
                generate_next<Index>(target, capture, move_list);
                capture.undo(target);
                capture.finish(jumper);
        }
}

template<bool Color, typename Rules, typename Board> template<size_t Index> 
void GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::generate_next(BitBoard jumper, Capture<Rules, Board>& capture, MoveList& move_list)
{
        PushAssign<Board, Index>()(jumper);
	if (!scan_next<Index>(jumper, capture, move_list) && capture.current_greater_equal_best()) {
		if (capture.current_not_equal_to_best())
			capture.improve_best(move_list);
		capture.template add_man_capture<Color>(jumper, move_list);
	}
}

// tag dispatching based on promotion condition
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_next(BitBoard jumper, Capture<Rules, Board>& capture, MoveList& move_list)
{
        return scan_next<Index>(jumper, capture, move_list, Int2Type<Variant::PromotionCondition<Rules>::VALUE>());
}

// partial specialization for men that promote on the back row
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_next(BitBoard jumper, Capture<Rules, Board>& capture, MoveList& move_list, Int2Type<Variant::PROMOTE_BR>)
{
        return scan_dirs<Index>(jumper, capture, move_list);
}

// partial specialization for men that promote en-passant
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_next(BitBoard jumper, Capture<Rules, Board>& capture, MoveList& move_list, Int2Type<Variant::PROMOTE_EP>)
{
        if (!capture.is_promotion<Color>(jumper))
                return scan_dirs<Index>(jumper, capture, move_list);
        else {
                capture.toggle_promotion();
                const bool found_next = GenerateTemplate<Color, Pieces::KING, Move::JUMPS, Rules, Board>::promote_en_passant<Index>(jumper, capture, move_list);
                capture.toggle_promotion();
                return found_next;
        }
}

// tag dispatching based on man capture directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jumper, Capture<Rules, Board>& capture, MoveList& move_list)
{
        return scan_dirs<Index>(jumper, capture, move_list, Int2Type<Variant::ManCaptureDirections<Rules>::VALUE>());
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jumper, Capture<Rules, Board>& capture, MoveList& move_list, Int2Type<Variant::DIRS_2>)
{
        return (
                scan_dir<MirrorDirIndex<Index>::U090>(jumper, capture, move_list) |
                scan_dir<Index>(jumper, capture, move_list)
        );
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jumper, Capture<Rules, Board>& capture, MoveList& move_list, Int2Type<Variant::DIRS_4>)
{
        return (
                scan_dir<RotateDirIndex<Index, R090>::VALUE>(jumper, capture, move_list) |
                scan_dir<RotateDirIndex<Index, L090>::VALUE>(jumper, capture, move_list) |
                scan_dir<Index>(jumper, capture, move_list)
        );
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_dirs(BitBoard jumper, Capture<Rules, Board>& capture, MoveList& move_list, Int2Type<Variant::DIRS_8>)
{
        return (
                scan_dir<RotateDirIndex<Index, R045>::VALUE>(jumper, capture, move_list) |
                scan_dir<RotateDirIndex<Index, L045>::VALUE>(jumper, capture, move_list) |
                scan_dir<RotateDirIndex<Index, R090>::VALUE>(jumper, capture, move_list) |
                scan_dir<RotateDirIndex<Index, L090>::VALUE>(jumper, capture, move_list) |
                scan_dir<RotateDirIndex<Index, R135>::VALUE>(jumper, capture, move_list) |
                scan_dir<RotateDirIndex<Index, L135>::VALUE>(jumper, capture, move_list) |
                scan_dir<Index>(jumper, capture, move_list)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan_dir(BitBoard jumper, Capture<Rules, Board>& capture, MoveList& move_list)
{
        PushAssign<Board, Index>()(jumper);
        return scan<Index>(jumper, capture, move_list);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::scan(BitBoard jumper, Capture<Rules, Board>& capture, MoveList& move_list)
{
        if (jumper & capture.template targets<Index>()) {
                capture.make(jumper);
                generate_next<Index>(jumper, capture, move_list);
                capture.undo(jumper);
                return true;
        } else
                return false;
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::count(const Position<Board>& p)
{
        MoveList move_list;
        generate(p, move_list);
        return move_list.size();
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect(const Position<Board>& p)
{
        return detect_targets(p);
}

// tag dispatching based on whether men can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_targets(const Position<Board>& p)
{
        return detect_targets(p, Int2Type<Variant::is_MenCaptureKings<Rules>::VALUE>());
}

// partial specialization for men that cannot capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_targets(const Position<Board>& p, Int2Type<false>)
{
        return detect_dirs(p.men(Color), p.men(!Color), p.not_occupied());
}

// partial specialization for men that can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_targets(const Position<Board>& p, Int2Type<true>)
{
        return detect_dirs(p.men(Color), p.pieces(!Color), p.not_occupied());
}

// tag dispatching based on man capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dirs(active_men, opponent_pieces, not_occupied, Int2Type<Variant::ManCaptureDirections<Rules>::VALUE>());
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variant::DIRS_2>)
{
        return (
                detect_dir<DirIndex<Board, Color>::LEFT_UP >(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_UP>(active_men, opponent_pieces, not_occupied)
        );
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variant::DIRS_4>)
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
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<Variant::DIRS_8>)
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
bool GenerateTemplate<Color, Pieces::PAWN, Move::JUMPS, Rules, Board>::detect_dir(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return !Bit::is_zero(Push<Board, Index>()(active_men) & opponent_pieces & Pull<Board, Index>()(not_occupied));
}
