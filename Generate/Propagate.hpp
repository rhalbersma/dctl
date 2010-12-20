#include "../Board/Direction.h"
#include "../Rules/CaptureTemplates.h"
#include "../Position/Move.h"
#include "../Utilities/Bit.h"
#include "../Utilities/Shift.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<typename Rules, typename Board> FORCE_INLINE
Propagate<Rules, Board>::Propagate(const Position<Board>& p)
:
	d_initial_targets(p.pieces(!p.to_move())),
	d_remaining_targets(d_initial_targets),
	d_not_occupied(p.not_occupied()),
	d_opponent_kings(p.kings(!p.to_move()))
{
        init<Rules>()(d_current_capture);
        init<Rules>()(d_best_capture);
        init_promotion();
}

template<typename Rules, typename Board> FORCE_INLINE
Propagate<Rules, Board>::Propagate(const Position<Board>& p, const Pieces& m)
{
}

// tag dispatching based on promotion condition
template<typename Rules, typename Board>
void Propagate<Rules, Board>::init_promotion(void)
{
        init_promotion(Int2Type<PromotionCondition<Rules>::VALUE>());
}

// partial specialization for men that promote on the back row
template<typename Rules, typename Board>
void Propagate<Rules, Board>::init_promotion(Int2Type<PROMOTE_BR>)
{
}

// partial specialization for men that promote en-passant
template<typename Rules, typename Board>
void Propagate<Rules, Board>::init_promotion(Int2Type<PROMOTE_EP>)
{
        d_promotion = false;
}

template<typename Rules, typename Board>
BitBoard Propagate<Rules, Board>::path(void) const
{
        return d_not_occupied;
}

template<typename Rules, typename Board> template<size_t Index>
BitBoard Propagate<Rules, Board>::path(void) const
{
        return path() & Board::JUMPABLE[Index];
}

template<typename Rules, typename Board> template<size_t Index> FORCE_INLINE
BitBoard Propagate<Rules, Board>::targets(void) const
{
        return d_remaining_targets & Shift<DirTraits<Index>::IS_NEGATIVE>()(path(), Board::DIR[Index]);
}

template<typename Rules, typename Board> template<size_t Index>
BitBoard Propagate<Rules, Board>::man_movers(void) const
{
        return Shift<DirTraits<Index>::IS_NEGATIVE>()(path(), Board::DIR[Index]);
}

template<typename Rules, typename Board> template<size_t Index>
BitBoard Propagate<Rules, Board>::man_jumpers(void) const
{
        return Shift<DirTraits<Index>::IS_NEGATIVE>()(targets<Index>(), Board::DIR[Index]);
}

template<typename Rules, typename Board> template<bool Color>
BitBoard Propagate<Rules, Board>::promotions(BitBoard dest_sq) const
{
        return dest_sq & Board::PROMOTION[Color][0];
}

template<typename Rules, typename Board>
BitBoard Propagate<Rules, Board>::captured_targets(void) const
{
        return d_initial_targets ^ d_remaining_targets;
}

// tag dispatching based on whether men can capture kings
template<typename Rules, typename Board>
BitBoard Propagate<Rules, Board>::captured_kings(BitBoard captured_pieces) const
{
        return captured_kings(captured_pieces, Int2Type<is_MenCaptureKings<Rules>::VALUE>());
}

// specialization for men that cannot capture kings
template<typename Rules, typename Board>
BitBoard Propagate<Rules, Board>::captured_kings(BitBoard, Int2Type<false>) const
{
        return 0;
}

// specialization for men that can capture kings
template<typename Rules, typename Board>
BitBoard Propagate<Rules, Board>::captured_kings(BitBoard captured_pieces, Int2Type<true>) const
{
        return captured_pieces & d_opponent_kings;
}

template<typename Rules, typename Board> template<bool Color>
bool Propagate<Rules, Board>::is_promotion(BitBoard dest_sq) const
{
        return !Bit::is_zero(promotions<Color>(dest_sq));
}

template<typename Rules, typename Board>
bool Propagate<Rules, Board>::is_promotion(void) const
{
        return d_promotion;
}

template<typename Rules, typename Board>
bool Propagate<Rules, Board>::current_greater_equal_best(void) const
{
        return greater_equal<Rules>()(d_current_capture, d_best_capture);
}

template<typename Rules, typename Board>
bool Propagate<Rules, Board>::current_not_equal_to_best(void) const
{
        return not_equal_to<Rules>()(d_current_capture, d_best_capture);
}

template<typename Rules, typename Board>
size_t Propagate<Rules, Board>::size(void) const
{
        return d_move_list.size();
}

template<typename Rules, typename Board>
const Pieces Propagate<Rules, Board>::operator[](size_t i) const
{
        return d_move_list[i];
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::toggle_opponent_kings(void)
{
        d_initial_targets = d_remaining_targets ^= d_opponent_kings;
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::toggle_with_king(void)
{
        d_current_capture.toggle_with_king();
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::toggle_promotion(void)
{
        d_promotion ^= TOGGLE;
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::launch(BitBoard jump_sq)
{
        d_from_sq = jump_sq;
        d_not_occupied ^= jump_sq;
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::finish(BitBoard jump_sq)
{
        d_not_occupied ^= jump_sq;
}

// tag dispatching on capture removal
template<typename Rules, typename Board> FORCE_INLINE
void Propagate<Rules, Board>::make(BitBoard target_sq)
{
        make(target_sq, Int2Type<CaptureRemoval<Rules>::VALUE>());
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Rules, typename Board>
void Propagate<Rules, Board>::make(BitBoard target_sq, Int2Type<REMOVE_1>)
{
        d_not_occupied ^= target_sq;
        make(target_sq, Int2Type<REMOVE_N>());
}

// partial specialization for complete removal after a capture sequence
template<typename Rules, typename Board> FORCE_INLINE
void Propagate<Rules, Board>::make(BitBoard target_sq, Int2Type<REMOVE_N>)
{
        d_remaining_targets ^= target_sq;
        increment<Rules>()(d_current_capture, target_sq, d_opponent_kings);
}

// tag dispatching on capture removal
template<typename Rules, typename Board> FORCE_INLINE
void Propagate<Rules, Board>::undo(BitBoard target_sq)
{
        undo(target_sq, Int2Type<CaptureRemoval<Rules>::VALUE>());
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Rules, typename Board> FORCE_INLINE
void Propagate<Rules, Board>::undo(BitBoard target_sq, Int2Type<REMOVE_1>)
{
        undo(target_sq, Int2Type<REMOVE_N>());
        d_not_occupied ^= target_sq;
}

// partial specialization for complete removal after a capture sequence
template<typename Rules, typename Board> FORCE_INLINE
void Propagate<Rules, Board>::undo(BitBoard target_sq, Int2Type<REMOVE_N>)
{
        decrement<Rules>()(d_current_capture, target_sq, d_opponent_kings);
        d_remaining_targets ^= target_sq;
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::improve_best(void)
{
        copy<Rules>()(d_best_capture, d_current_capture);
        d_move_list.clear();
}

template<typename Rules, typename Board> template<bool Color>
void Propagate<Rules, Board>::add_king_move(BitBoard delta)
{
        Move::push_back<Color>(d_move_list, delta);
}

template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_man_move(BitBoard start_sq, BitBoard dest_sq)
{
        Move::push_back<Color>(d_move_list, start_sq ^ dest_sq, promotions<Color>(dest_sq));
}

// tag dispatching based on ambiguity of man captures
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_man_capture(BitBoard dest_sq)
{
        add_man_capture<Color>(dest_sq, Int2Type<is_AmbiguousManCapture<Rules>::VALUE>());
}

// partial specialization for man captures that are unambiguous
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_man_capture(BitBoard dest_sq, Int2Type<false>)
{
        const BitBoard captured_pieces = captured_targets();
        Move::push_back<Color, Rules>(d_move_list, d_from_sq ^ dest_sq, promotions<Color>(dest_sq), captured_pieces, captured_kings(captured_pieces));
}

// partial specialization for man captures that can be ambiguous
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_man_capture(BitBoard dest_sq, Int2Type<true>)
{
        const BitBoard captured_pieces = captured_targets();
        const bool ambiguous = !d_move_list.empty() && large<Rules>()(d_current_capture, captured_pieces);
        Move::push_back<Color, Rules>(d_move_list, d_from_sq ^ dest_sq, promotions<Color>(dest_sq), captured_pieces, captured_kings(captured_pieces));
        if (ambiguous && Move::non_unique_back<Rules>(d_move_list))
                d_move_list.pop_back();
}

// tag dispatching based on king halt after final capture
template<typename Rules, typename Board> template<bool Color, size_t Index> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq)
{
        add_king_capture<Color, Index>(dest_sq, Int2Type<KingCaptureHalt<Rules>::VALUE>());
}

// partial specialization for kings that halt immediately if the final capture is a king, and slide through otherwise
template<typename Rules, typename Board> template<bool Color, size_t Index> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, Int2Type<HALT_K>)
{
        if (d_opponent_kings & (Shift<DirTraits<Index>::IS_NEGATIVE>()(dest_sq, Board::DIR[Index])))
                add_king_capture<Color, Index>(dest_sq, Int2Type<HALT_1>());
        else
                add_king_capture<Color, Index>(dest_sq, Int2Type<HALT_N>());
}

// partial specialization for kings that halt immediately after the final capture
template<typename Rules, typename Board> template<bool Color, size_t Index> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, Int2Type<HALT_1>)
{
        const BitBoard captured_pieces = captured_targets();
        const bool ambiguous = !d_move_list.empty() && large<Rules>()(d_current_capture, captured_pieces);
        add_king_capture<Color>(dest_sq, captured_pieces, captured_pieces & d_opponent_kings, ambiguous);
}

// partial specialization for kings that slide through after the final capture
template<typename Rules, typename Board> template<bool Color, size_t Index> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, Int2Type<HALT_N>)
{
        assert(dest_sq & path());

        const BitBoard captured_pieces = captured_targets();
        const BitBoard captured_kings = captured_pieces & d_opponent_kings;
        const bool ambiguous = !d_move_list.empty() && large<Rules>()(d_current_capture, captured_pieces);
        do {
                add_king_capture<Color>(dest_sq, captured_pieces, captured_kings, ambiguous);
                ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(dest_sq, Board::DIR[Index]);
        } while (dest_sq & path());
}

// tag dispatching based on promotion condition
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, BitBoard captured_pieces, BitBoard captured_kings, bool ambiguous)
{
        add_king_capture<Color>(dest_sq, captured_pieces, captured_kings, Int2Type<PromotionCondition<Rules>::VALUE>());
        if (ambiguous && Move::non_unique_back<Rules>(d_move_list))
                d_move_list.pop_back();
}

// partial specialization for men that promote on the back row
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, BitBoard captured_pieces, BitBoard captured_kings, Int2Type<PROMOTE_BR>)
{
        Move::push_back<Color, Rules>(d_move_list, d_from_sq ^ dest_sq, captured_pieces, captured_kings);
}

// partial specialization for men that promote en-passant
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, BitBoard captured_pieces, BitBoard captured_kings, Int2Type<PROMOTE_EP>)
{
        if (!is_promotion())
                Move::push_back<Color, Rules>(d_move_list, d_from_sq ^ dest_sq,          captured_pieces, captured_kings);
        else
                Move::push_back<Color, Rules>(d_move_list, d_from_sq ^ dest_sq, dest_sq, captured_pieces, captured_kings);
}
