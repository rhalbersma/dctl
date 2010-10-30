#include "Propagate.h"
#include "../Board/Board.h"
#include "../Board/Direction.h"
#include "../Board/Transform.h"
#include "../Position/Position.h"
#include "../Utilities/Bit.h"
#include "../Utilities/InlineOptions.h"
#include "../Utilities/Shift.h"
#include <cassert>

template<typename Rules, typename Board>
void KingCaptures::generate(const Position<Board>& p, Propagate<Rules, Board>& capture)
{
        if (p.to_move())
                generate<Side::WHITE>(p, capture);
        else
                generate<Side::BLACK>(p, capture);
}

template<bool Color, typename Rules, typename Board>
void KingCaptures::generate(const Position<Board>& p, Propagate<Rules, Board>& capture)
{
        assert(p.to_move() == Color);
        generate_precede<Color>(p, capture);
}

// tag dispatching based on relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void KingCaptures::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture)
{
        generate_precede<Color>(p, capture, Int2Type<is_RelativeKingPrecedence<Rules>::VALUE>());
}

// partial specialization for relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void KingCaptures::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture, Int2Type<true>)
{
        capture.toggle_with_king();
        generate_precede<Color>(p, capture, Int2Type<false>());
        capture.toggle_with_king();
}

// partial specialization for no relative king capture precedence
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void KingCaptures::generate_precede(const Position<Board>& p, Propagate<Rules, Board>& capture, Int2Type<false>)
{
        generate_serial<Color>(p.kings(Color), capture);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void KingCaptures::generate_serial(BitBoard active_kings, Propagate<Rules, Board>& capture)
{
        assert(!Bit::is_zero(active_kings));
        BitBoard jump_sq;
        do {
                jump_sq = Bit::get_lowest(active_kings);
                capture.launch(jump_sq);
                generate_dirs<Color>(jump_sq, capture);
                capture.finish(jump_sq);
                Bit::clear_lowest(active_kings);
        } while (active_kings);
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void KingCaptures::generate_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        generate_dirs<Color>(jump_sq, capture, Int2Type<KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void KingCaptures::generate_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<DIRS_4>)
{
        generate_dir<Color, DirIndex<Color>::LU>(jump_sq, capture);
        generate_dir<Color, DirIndex<Color>::RU>(jump_sq, capture);
        generate_dir<Color, DirIndex<Color>::LD>(jump_sq, capture);
        generate_dir<Color, DirIndex<Color>::RD>(jump_sq, capture);
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void KingCaptures::generate_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<DIRS_8>)
{
        generate_dir<Color, DirIndex<Color>::LU>(jump_sq, capture);
        generate_dir<Color, DirIndex<Color>::RU>(jump_sq, capture);
        generate_dir<Color, DirIndex<Color>::LD>(jump_sq, capture);
        generate_dir<Color, DirIndex<Color>::RD>(jump_sq, capture);
        generate_dir<Color, DirIndex<Color>::L >(jump_sq, capture);
        generate_dir<Color, DirIndex<Color>::R >(jump_sq, capture);
        generate_dir<Color, DirIndex<Color>::U >(jump_sq, capture);
        generate_dir<Color, DirIndex<Color>::D >(jump_sq, capture);
}

// tag dispatching based on king range
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
void KingCaptures::generate_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return generate_dir<Color, Index>(jump_sq, capture, Int2Type<is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
void KingCaptures::generate_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_1>)
{
        ShiftAssign<Direction<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
        if (jump_sq & capture.template targets<Index>()) {
                capture.make(jump_sq);
                generate_next<Color, Index>(jump_sq, capture);
                capture.undo(jump_sq);
        }
}

// partial specialization for long ranged kings
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
void KingCaptures::generate_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_N>)
{
        do ShiftAssign<Direction<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]); while (jump_sq & capture.path());
        if (jump_sq & capture.template targets<Index>()) {
                capture.make(jump_sq);
                generate_next<Color, Index>(jump_sq, capture);
                capture.undo(jump_sq);
        }
}

template<bool Color, size_t Index, typename Rules, typename Board>
void KingCaptures::generate_next(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        ShiftAssign<Direction<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
        if (!scan_next<Color, Index>(jump_sq, capture) && capture.current_greater_equal_best()) {
		if (capture.current_not_equal_to_best())
			capture.improve_best();
		capture.template add_king_capture<Color, Index>(jump_sq);
        }
}

// tag dispatching based on king range
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_next<Color, Index>(jump_sq, capture, Int2Type<is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_1>)
{
        return (
                scan_dirs<Color, Index>(jump_sq, capture) |
                scan_dir<Color, Index>(jump_sq, capture)
        );
}

// partial specialization for long ranged kings
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_N>)
{
        return scan_long<Color, Index>(jump_sq, capture);
}

// tag dispatching based on king capture direction reversal
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_long(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_long<Color, Index>(jump_sq, capture, Int2Type<is_DirectionReversal<Rules>::VALUE>());
}

// partial specialization for kings that cannot reverse their capture direction
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_long(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<false>)
{
        return scan_forward<Color, Index>(jump_sq, capture);
}

// partial specialization for kings that can reverse their capture direction
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_long(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<true>)
{
        return (
                scan_reverse<Color, Index>(jump_sq, capture) |
                scan_forward<Color, Index>(jump_sq, capture)
        );
}

template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_reverse(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_dir<Color, RotateDirection<Index, D180>::VALUE>(jump_sq, capture);
}

template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_forward(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        assert(jump_sq & capture.path());
        bool found_capture = false;
        do {
                found_capture |= scan_dirs<Color, Index>(jump_sq, capture);
                ShiftAssign<Direction<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
        } while (jump_sq & capture.path());
        return found_capture |= scan<Color, Index>(jump_sq, capture);
}

// tag dispatching based on king capture directions
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_dirs<Color, Index>(jump_sq, capture, Int2Type<KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<DIRS_4>)
{
        return (
                scan_dir<Color, RotateDirection<Index, R090>::VALUE>(jump_sq, capture) |
                scan_dir<Color, RotateDirection<Index, L090>::VALUE>(jump_sq, capture)
        );
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<DIRS_8>)
{
        return (
                scan_dir<Color, RotateDirection<Index, R045>::VALUE>(jump_sq, capture) |
                scan_dir<Color, RotateDirection<Index, L045>::VALUE>(jump_sq, capture) |
                scan_dir<Color, RotateDirection<Index, R090>::VALUE>(jump_sq, capture) |
                scan_dir<Color, RotateDirection<Index, L090>::VALUE>(jump_sq, capture) |
                scan_dir<Color, RotateDirection<Index, R135>::VALUE>(jump_sq, capture) |
                scan_dir<Color, RotateDirection<Index, L135>::VALUE>(jump_sq, capture)
        );
}

// tag dispatching based on king range
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_dir<Color, Index>(jump_sq, capture, Int2Type<is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_1>)
{
        ShiftAssign<Direction<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
        return scan<Color, Index>(jump_sq, capture);
}

// partial specialization for long ranged kings
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan_dir(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<RANGE_N>)
{
        do ShiftAssign<Direction<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]); while (jump_sq & capture.template path<Index>());
        return scan<Color, Index>(jump_sq, capture);
}

template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::scan(BitBoard target_sq, Propagate<Rules, Board>& capture)
{
        if (target_sq & capture.template targets<Index>()) {
                capture.make(target_sq);
                generate_next<Color, Index>(target_sq, capture);
                capture.undo(target_sq);
                return true;
        } else
                return false;
}

template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::promotions_en_passant(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_next<Color, Index>(jump_sq, capture);
}

template<typename Rules, typename Board> FORCE_INLINE
size_t KingCaptures::count(const Position<Board>& p)
{
        if (p.to_move())
                return count<Side::WHITE, Rules>(p);
        else
                return count<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t KingCaptures::count(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        Propagate<Rules, Board> captures(p);
        generate<Color>(p, captures);
        return captures.size();
}

template<typename Rules, typename Board>
bool KingCaptures::detect(const Position<Board>& p)
{
        if (p.to_move())
                return detect<Side::WHITE, Rules>(p);
        else
                return detect<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board>
bool KingCaptures::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return detect_dirs<Color, Rules, Board>(p.kings(Color), p.pieces(!Color), p.not_occupied());
}

// tag dispatching based on king capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dirs<Color, Rules, Board>(active_kings, opponent_pieces, not_occupied, Int2Type<KingCaptureDirections<Rules>::VALUE>());
}

// partial specialization for kings that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<DIRS_4>)
{
        return (
                detect_dir<DirIndex<Color>::LU, Rules, Board>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::RU, Rules, Board>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::LD, Rules, Board>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::RD, Rules, Board>(active_kings, opponent_pieces, not_occupied)
        );
}

// partial specialization for kings that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::detect_dirs(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<DIRS_8>)
{
        return (
                detect_dir<DirIndex<Color>::LU, Rules, Board>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::RU, Rules, Board>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::LD, Rules, Board>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::RD, Rules, Board>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::L,  Rules, Board>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::R,  Rules, Board>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::U,  Rules, Board>(active_kings, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::D,  Rules, Board>(active_kings, opponent_pieces, not_occupied)
        );
}

// tag dispatching based on king range
template<size_t Index, typename Rules, typename Board> FORCE_INLINE
bool KingCaptures::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dir<Index, Board>(active_kings, opponent_pieces, not_occupied, Int2Type<is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<size_t Index, typename Board> FORCE_INLINE
bool KingCaptures::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<RANGE_1>)
{
        return !Bit::is_zero(Shift<Direction<Index>::IS_POSITIVE>()(active_kings, Board::DIR[Index]) & opponent_pieces & Shift<Direction<Index>::IS_NEGATIVE>()(not_occupied, Board::DIR[Index]));
}

// partial specialization for long ranged kings
template<size_t Index, typename Board> FORCE_INLINE
bool KingCaptures::detect_dir(BitBoard active_kings, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<RANGE_N>)
{
        return !Bit::is_zero(Shift<Direction<Index>::IS_POSITIVE>()(Bit::flood_fill<Direction<Index>::IS_POSITIVE>(active_kings, not_occupied, Board::DIR[Index]), Board::DIR[Index]) & opponent_pieces & Shift<Direction<Index>::IS_NEGATIVE>()(not_occupied, Board::DIR[Index]));
}
