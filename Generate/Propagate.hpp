#include "../Board/Direction.h"
#include "../Variant/CaptureTemplates.h"
#include "../Position/Move.h"
#include "../Utilities/Bit.h"
#include "../Utilities/Shift.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<typename Rules, typename Board> FORCE_INLINE
Propagate<Rules, Board>::Propagate(const Position<Board>& p)
:
	initial_targets_(p.pieces(!p.to_move())),
	remaining_targets_(initial_targets_),
	not_occupied_(p.not_occupied()),
	opponent_kings_(p.kings(!p.to_move()))
{
        init<Rules>()(current_capture_);
        init<Rules>()(best_capture_);
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
        init_promotion(Int2Type<Variant::PromotionCondition<Rules>::VALUE>());
}

// partial specialization for men that promote on the back row
template<typename Rules, typename Board>
void Propagate<Rules, Board>::init_promotion(Int2Type<Variant::PROMOTE_BR>)
{
}

// partial specialization for men that promote en-passant
template<typename Rules, typename Board>
void Propagate<Rules, Board>::init_promotion(Int2Type<Variant::PROMOTE_EP>)
{
        promotion_ = false;
}

template<typename Rules, typename Board>
BitBoard Propagate<Rules, Board>::path(void) const
{
        return not_occupied_;
}

template<typename Rules, typename Board> template<size_t Index>
BitBoard Propagate<Rules, Board>::path(void) const
{
        return path() & Board::JUMPABLE[Index];
}

template<typename Rules, typename Board> template<size_t Index> FORCE_INLINE
BitBoard Propagate<Rules, Board>::targets(void) const
{
        return remaining_targets_ & Shift<DirTraits<Index>::IS_NEGATIVE>()(path(), Board::DIR[Index]);
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
        return initial_targets_ ^ remaining_targets_;
}

// tag dispatching based on whether men can capture kings
template<typename Rules, typename Board>
BitBoard Propagate<Rules, Board>::captured_kings(BitBoard captured_pieces) const
{
        return captured_kings(captured_pieces, Int2Type<Variant::is_MenCaptureKings<Rules>::VALUE>());
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
        return captured_pieces & opponent_kings_;
}

template<typename Rules, typename Board> template<bool Color>
bool Propagate<Rules, Board>::is_promotion(BitBoard dest_sq) const
{
        return !Bit::is_zero(promotions<Color>(dest_sq));
}

template<typename Rules, typename Board>
bool Propagate<Rules, Board>::is_promotion(void) const
{
        return promotion_;
}

template<typename Rules, typename Board>
bool Propagate<Rules, Board>::current_greater_equal_best(void) const
{
        return greater_equal<Rules>()(current_capture_, best_capture_);
}

template<typename Rules, typename Board>
bool Propagate<Rules, Board>::current_not_equal_to_best(void) const
{
        return not_equal_to<Rules>()(current_capture_, best_capture_);
}

template<typename Rules, typename Board>
size_t Propagate<Rules, Board>::size(void) const
{
        return move_list_.size();
}

template<typename Rules, typename Board>
const Pieces Propagate<Rules, Board>::operator[](size_t i) const
{
        return move_list_[i];
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::toggle_opponent_kings(void)
{
        initial_targets_ = remaining_targets_ ^= opponent_kings_;
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::toggle_with_king(void)
{
        current_capture_.toggle_with_king();
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::toggle_promotion(void)
{
        promotion_ ^= TOGGLE;
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::launch(BitBoard jump_sq)
{
        from_sq_ = jump_sq;
        not_occupied_ ^= jump_sq;
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::finish(BitBoard jump_sq)
{
        not_occupied_ ^= jump_sq;
}

// tag dispatching on capture removal
template<typename Rules, typename Board> FORCE_INLINE
void Propagate<Rules, Board>::make(BitBoard target_sq)
{
        make(target_sq, Int2Type<Variant::CaptureRemoval<Rules>::VALUE>());
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Rules, typename Board>
void Propagate<Rules, Board>::make(BitBoard target_sq, Int2Type<Variant::REMOVE_1>)
{
        not_occupied_ ^= target_sq;
        make(target_sq, Int2Type<Variant::REMOVE_N>());
}

// partial specialization for complete removal after a capture sequence
template<typename Rules, typename Board> FORCE_INLINE
void Propagate<Rules, Board>::make(BitBoard target_sq, Int2Type<Variant::REMOVE_N>)
{
        remaining_targets_ ^= target_sq;
        increment<Rules>()(current_capture_, target_sq, opponent_kings_);
}

// tag dispatching on capture removal
template<typename Rules, typename Board> FORCE_INLINE
void Propagate<Rules, Board>::undo(BitBoard target_sq)
{
        undo(target_sq, Int2Type<Variant::CaptureRemoval<Rules>::VALUE>());
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Rules, typename Board> FORCE_INLINE
void Propagate<Rules, Board>::undo(BitBoard target_sq, Int2Type<Variant::REMOVE_1>)
{
        undo(target_sq, Int2Type<Variant::REMOVE_N>());
        not_occupied_ ^= target_sq;
}

// partial specialization for complete removal after a capture sequence
template<typename Rules, typename Board> FORCE_INLINE
void Propagate<Rules, Board>::undo(BitBoard target_sq, Int2Type<Variant::REMOVE_N>)
{
        decrement<Rules>()(current_capture_, target_sq, opponent_kings_);
        remaining_targets_ ^= target_sq;
}

template<typename Rules, typename Board>
void Propagate<Rules, Board>::improve_best(void)
{
        copy<Rules>()(best_capture_, current_capture_);
        move_list_.clear();
}

template<typename Rules, typename Board> template<bool Color>
void Propagate<Rules, Board>::add_king_move(BitBoard delta)
{
        Move::push_back<Color>(move_list_, delta);
}

template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_man_move(BitBoard start_sq, BitBoard dest_sq)
{
        Move::push_back<Color>(move_list_, start_sq ^ dest_sq, promotions<Color>(dest_sq));
}

// tag dispatching based on ambiguity of man captures
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_man_capture(BitBoard dest_sq)
{
        add_man_capture<Color>(dest_sq, Int2Type<Variant::is_AmbiguousManCapture<Rules>::VALUE>());
}

// partial specialization for man captures that are unambiguous
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_man_capture(BitBoard dest_sq, Int2Type<false>)
{
        const BitBoard captured_pieces = captured_targets();
        Move::push_back<Color, Rules>(move_list_, from_sq_ ^ dest_sq, promotions<Color>(dest_sq), captured_pieces, captured_kings(captured_pieces));
}

// partial specialization for man captures that can be ambiguous
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_man_capture(BitBoard dest_sq, Int2Type<true>)
{
        const BitBoard captured_pieces = captured_targets();
        const bool ambiguous = !move_list_.empty() && large<Rules>()(current_capture_, captured_pieces);
        Move::push_back<Color, Rules>(move_list_, from_sq_ ^ dest_sq, promotions<Color>(dest_sq), captured_pieces, captured_kings(captured_pieces));
        if (ambiguous && Move::non_unique_back<Rules>(move_list_))
                move_list_.pop_back();
}

// tag dispatching based on king halt after final capture
template<typename Rules, typename Board> template<bool Color, size_t Index> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq)
{
        add_king_capture<Color, Index>(dest_sq, Int2Type<Variant::KingCaptureHalt<Rules>::VALUE>());
}

// partial specialization for kings that halt immediately if the final capture is a king, and slide through otherwise
template<typename Rules, typename Board> template<bool Color, size_t Index> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, Int2Type<Variant::HALT_K>)
{
        if (opponent_kings_ & (Shift<DirTraits<Index>::IS_NEGATIVE>()(dest_sq, Board::DIR[Index])))
                add_king_capture<Color, Index>(dest_sq, Int2Type<Variant::HALT_1>());
        else
                add_king_capture<Color, Index>(dest_sq, Int2Type<Variant::HALT_N>());
}

// partial specialization for kings that halt immediately after the final capture
template<typename Rules, typename Board> template<bool Color, size_t Index> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, Int2Type<Variant::HALT_1>)
{
        const BitBoard captured_pieces = captured_targets();
        const bool ambiguous = !move_list_.empty() && large<Rules>()(current_capture_, captured_pieces);
        add_king_capture<Color>(dest_sq, captured_pieces, captured_pieces & opponent_kings_, ambiguous);
}

// partial specialization for kings that slide through after the final capture
template<typename Rules, typename Board> template<bool Color, size_t Index> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, Int2Type<Variant::HALT_N>)
{
        assert(dest_sq & path());

        const BitBoard captured_pieces = captured_targets();
        const BitBoard captured_kings = captured_pieces & opponent_kings_;
        const bool ambiguous = !move_list_.empty() && large<Rules>()(current_capture_, captured_pieces);
        do {
                add_king_capture<Color>(dest_sq, captured_pieces, captured_kings, ambiguous);
                ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(dest_sq, Board::DIR[Index]);
        } while (dest_sq & path());
}

// tag dispatching based on promotion condition
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, BitBoard captured_pieces, BitBoard captured_kings, bool ambiguous)
{
        add_king_capture<Color>(dest_sq, captured_pieces, captured_kings, Int2Type<Variant::PromotionCondition<Rules>::VALUE>());
        if (ambiguous && Move::non_unique_back<Rules>(move_list_))
                move_list_.pop_back();
}

// partial specialization for men that promote on the back row
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, BitBoard captured_pieces, BitBoard captured_kings, Int2Type<Variant::PROMOTE_BR>)
{
        Move::push_back<Color, Rules>(move_list_, from_sq_ ^ dest_sq, captured_pieces, captured_kings);
}

// partial specialization for men that promote en-passant
template<typename Rules, typename Board> template<bool Color> FORCE_INLINE
void Propagate<Rules, Board>::add_king_capture(BitBoard dest_sq, BitBoard captured_pieces, BitBoard captured_kings, Int2Type<Variant::PROMOTE_EP>)
{
        if (!is_promotion())
                Move::push_back<Color, Rules>(move_list_, from_sq_ ^ dest_sq,          captured_pieces, captured_kings);
        else
                Move::push_back<Color, Rules>(move_list_, from_sq_ ^ dest_sq, dest_sq, captured_pieces, captured_kings);
}
