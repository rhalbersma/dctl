#include "KingCaptures.h"
#include "Propagate.h"
#include "../Board/Board.h"
#include "../Board/Direction.h"
#include "../Position/Position.h"
#include "../Utilities/Bit.h"
#include "../Utilities/InlineOptions.h"
#include "../Utilities/Shift.h"
#include <cassert>

template<typename Rules, typename Board>
void ManCaptures::generate(const Position<Board>& p, Propagate<Rules, Board>& capture)
{
        if (p.to_move())
                generate<Side::WHITE>(p, capture);
        else
                generate<Side::BLACK>(p, capture);
}

template<bool Color, typename Rules, typename Board>
void ManCaptures::generate(const Position<Board>& p, Propagate<Rules, Board>& capture)
{
        assert(p.to_move() == Color);
        generate_targets<Color>(p, capture);
}

// tag dispatching based on whether men can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManCaptures::generate_targets(const Position<Board>& p, Propagate<Rules, Board>& capture)
{
        generate_targets<Color>(p, capture, Int2Type<is_MenCaptureKings<Rules>::VALUE>());
}

// partial specialization for men that cannot capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManCaptures::generate_targets(const Position<Board>& p, Propagate<Rules, Board>& capture, Int2Type<false>)
{
        capture.toggle_opponent_kings();
        generate_targets<Color>(p, capture, Int2Type<true>());
        capture.toggle_opponent_kings();
}

// partial specialization for men that can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManCaptures::generate_targets(const Position<Board>& p, Propagate<Rules, Board>& capture, Int2Type<true>)
{
        generate_dirs<Color>(p.men(Color), capture);
}

// tag dispatching based on man capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManCaptures::generate_dirs(BitBoard active_men, Propagate<Rules, Board>& capture)
{
        generate_dirs<Color>(active_men, capture, Int2Type<ManCaptureDirections<Rules>::VALUE>());
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManCaptures::generate_dirs(BitBoard active_men, Propagate<Rules, Board>& capture, Int2Type<DIRS_2>)
{
        generate_dir<Color, DirIndex<Color>::LU>(active_men, capture);
        generate_dir<Color, DirIndex<Color>::RU>(active_men, capture);
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManCaptures::generate_dirs(BitBoard active_men, Propagate<Rules, Board>& capture, Int2Type<DIRS_4>)
{
        generate_dir<Color, DirIndex<Color>::LU>(active_men, capture);
        generate_dir<Color, DirIndex<Color>::RU>(active_men, capture);
        generate_dir<Color, DirIndex<Color>::LD>(active_men, capture);
        generate_dir<Color, DirIndex<Color>::RD>(active_men, capture);
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManCaptures::generate_dirs(BitBoard active_men, Propagate<Rules, Board>& capture, Int2Type<DIRS_8>)
{
        generate_dir<Color, DirIndex<Color>::LU>(active_men, capture);
        generate_dir<Color, DirIndex<Color>::RU>(active_men, capture);
        generate_dir<Color, DirIndex<Color>::LD>(active_men, capture);
        generate_dir<Color, DirIndex<Color>::RD>(active_men, capture);
        generate_dir<Color, DirIndex<Color>::L >(active_men, capture);
        generate_dir<Color, DirIndex<Color>::R >(active_men, capture);
        generate_dir<Color, DirIndex<Color>::U >(active_men, capture);
        generate_dir<Color, DirIndex<Color>::D >(active_men, capture);
}

template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
void ManCaptures::generate_dir(BitBoard active_men, Propagate<Rules, Board>& capture)
{
        BitBoard jump_sq, target_sq;
        for (active_men &= capture.template jumpers<Index>(); active_men; Bit::clear_lowest(active_men)) {
                jump_sq = Bit::get_lowest(active_men);
                target_sq = Shift<Direction<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
                capture.launch(jump_sq);
                capture.make(target_sq);
                generate_next<Color, Index>(target_sq, capture);
                capture.undo(target_sq);
                capture.finish(jump_sq);
        }
}

template<bool Color, size_t Index, typename Rules, typename Board>
void ManCaptures::generate_next(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        ShiftAssign<Direction<Index>::IS_POSITIVE>()(jump_sq, Board::DIR[Index]);
	if (!scan_next<Color, Index>(jump_sq, capture) && capture.current_greater_equal_best()) {
		if (capture.current_not_equal_to_best())
			capture.improve_best();
		capture.template add_man_capture<Color>(jump_sq);
	}
}

// tag dispatching based on promotion rule
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_next<Color, Index>(jump_sq, capture, Int2Type<is_PromotionEnPassant<Rules>::VALUE>());
}

// partial specialization for men that promote on the back row
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<PROMOTE_BR>)
{
        return ManCaptures::scan_dirs<Color, Index>(jump_sq, capture);
}

// partial specialization for men that promote en-passant
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::scan_next(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<PROMOTE_EP>)
{
        if (!capture.is_promotion<Color>(jump_sq))
                return ManCaptures::scan_dirs<Color, Index>(jump_sq, capture);
        else {
                capture.toggle_promotion();
                const bool found_next = KingCaptures::promotions_en_passant<Color, Index>(jump_sq, capture);
                capture.toggle_promotion();
                return found_next;
        }
}

// tag dispatching based on man capture directions
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture)
{
        return scan_dirs<Color, Index>(jump_sq, capture, Int2Type<ManCaptureDirections<Rules>::VALUE>());
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<DIRS_2>)
{
        return (
                scan_dir<Color, Direction<Index>::PROJECT_U90>(jump_sq, capture) |
                scan_dir<Color, Index>(jump_sq, capture)
        );
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<DIRS_4>)
{
        return (
                scan_dir<Color, Direction<Index>::ROTATE_R090>(jump_sq, capture) |
                scan_dir<Color, Direction<Index>::ROTATE_L090>(jump_sq, capture) |
                scan_dir<Color, Index>(jump_sq, capture)
        );
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::scan_dirs(BitBoard jump_sq, Propagate<Rules, Board>& capture, Int2Type<DIRS_8>)
{
        return (
                scan_dir<Color, Direction<Index>::ROTATE_R045>(jump_sq, capture) |
                scan_dir<Color, Direction<Index>::ROTATE_L045>(jump_sq, capture) |
                scan_dir<Color, Direction<Index>::ROTATE_R090>(jump_sq, capture) |
                scan_dir<Color, Direction<Index>::ROTATE_L090>(jump_sq, capture) |
                scan_dir<Color, Direction<Index>::ROTATE_R135>(jump_sq, capture) |
                scan_dir<Color, Direction<Index>::ROTATE_L135>(jump_sq, capture) |
                scan_dir<Color, Index>(jump_sq, capture)
        );
}

template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::scan_dir(BitBoard target_sq, Propagate<Rules, Board>& capture)
{
        ShiftAssign<Direction<Index>::IS_POSITIVE>()(target_sq, Board::DIR[Index]);
        return scan<Color, Index>(target_sq, capture);
}

template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::scan(BitBoard target_sq, Propagate<Rules, Board>& capture)
{
        if (target_sq & capture.template targets<Index>()) {
                capture.make(target_sq);
                generate_next<Color, Index>(target_sq, capture);
                capture.undo(target_sq);
                return true;
        } else
                return false;
}

template<typename Rules, typename Board> FORCE_INLINE
size_t ManCaptures::count(const Position<Board>& p)
{
        if (p.to_move())
                return count<Side::WHITE, Rules>(p);
        else
                return count<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t ManCaptures::count(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        Propagate<Rules, Board> captures(p);
        generate<Color>(p, captures);
        return captures.size();
}

template<typename Rules, typename Board>
bool ManCaptures::detect(const Position<Board>& p)
{
        if (p.to_move())
                return detect<Side::WHITE, Rules>(p);
        else
                return detect<Side::BLACK, Rules>(p);
}

template<bool Color, typename Rules, typename Board>
bool ManCaptures::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return detect_targets<Color, Rules>(p);
}

// tag dispatching based on whether men can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::detect_targets(const Position<Board>& p)
{
        return detect_targets<Color, Rules>(p, Int2Type<is_MenCaptureKings<Rules>::VALUE>());
}

// partial specialization for men that cannot capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::detect_targets(const Position<Board>& p, Int2Type<false>)
{
        return detect_dirs<Color, Rules, Board>(p.men(Color), p.men(!Color), p.not_occupied());
}

// partial specialization for men that can capture kings
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::detect_targets(const Position<Board>& p, Int2Type<true>)
{
        return detect_dirs<Color, Rules, Board>(p.men(Color), p.pieces(!Color), p.not_occupied());
}

// tag dispatching based on man capture directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return detect_dirs<Color, Rules, Board>(active_men, opponent_pieces, not_occupied, Int2Type<ManCaptureDirections<Rules>::VALUE>());
}

// partial specialization for men that capture in the 2 forward diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<DIRS_2>)
{
        return (
                detect_dir<DirIndex<Color>::LU, Board>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::RU, Board>(active_men, opponent_pieces, not_occupied)
        );
}

// partial specialization for men that capture in the 4 diagonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<DIRS_4>)
{
        return (
                detect_dir<DirIndex<Color>::LU, Board>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::RU, Board>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::LD, Board>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::RD, Board>(active_men, opponent_pieces, not_occupied)
        );
}

// partial specialization for men that capture in the 8 diagonal and orthogonal directions
template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool ManCaptures::detect_dirs(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied, Int2Type<DIRS_8>)
{
        return (
                detect_dir<DirIndex<Color>::LU, Board>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::RU, Board>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::LD, Board>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::RD, Board>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::L,  Board>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::R,  Board>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::U,  Board>(active_men, opponent_pieces, not_occupied) ||
                detect_dir<DirIndex<Color>::D,  Board>(active_men, opponent_pieces, not_occupied)
        );
}

template<size_t Index, typename Board> FORCE_INLINE
bool ManCaptures::detect_dir(BitBoard active_men, BitBoard opponent_pieces, BitBoard not_occupied)
{
        return !Bit::is_zero(Shift<Direction<Index>::IS_POSITIVE>()(active_men, Board::DIR[Index]) & opponent_pieces & Shift<Direction<Index>::IS_NEGATIVE>()(not_occupied, Board::DIR[Index]));
}
