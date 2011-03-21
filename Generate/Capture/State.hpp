#include "Templates.h"
#include "../../Geometry/Direction.h"
#include "../../Geometry/PushPull.h"
#include "../../Position/Move.h"
#include "../../Utilities/Bit.h"
#include <cassert>

using namespace Geometry::Direction;

namespace Capture { 

template<typename Rules, typename Board>
State<Rules, Board>::State(const Position<Board>& p)
:
	initial_targets_(p.pieces(!p.to_move())),
	remaining_targets_(initial_targets_),
	not_occupied_(p.not_occupied()),
	king_targets_(p.kings(!p.to_move()))
{
        init<Rules>()(current_);
        init<Rules>()(best_);
}

template<typename Rules, typename Board> template<size_t Index>
BitBoard State<Rules, Board>::targets(void) const
{
        return remaining_targets_ & Pull<Board, Index>()(path());
}

template<typename Rules, typename Board> template<size_t Index>
BitBoard State<Rules, Board>::path(void) const
{
        return path() & Board::JUMPABLE[Index];
}

template<typename Rules, typename Board>
BitBoard State<Rules, Board>::path(void) const
{
        return not_occupied_;
}

template<typename Rules, typename Board> template<bool Color>
BitBoard State<Rules, Board>::promotions(BitBoard dest_sq) const
{
        return dest_sq & Board::PROMOTION[Color][0];
}

template<typename Rules, typename Board>
BitBoard State<Rules, Board>::captured_targets(void) const
{
        return initial_targets_ ^ remaining_targets_;
}

// tag dispatching based on whether men can capture kings
template<typename Rules, typename Board>
BitBoard State<Rules, Board>::captured_kings(BitBoard captured_pieces) const
{
        return captured_kings(captured_pieces, Int2Type<Variants::is_MenCaptureKings<Rules>::VALUE>());
}

// specialization for men that cannot capture kings
template<typename Rules, typename Board>
BitBoard State<Rules, Board>::captured_kings(BitBoard, Int2Type<false>) const
{
        return 0;
}

// specialization for men that can capture kings
template<typename Rules, typename Board>
BitBoard State<Rules, Board>::captured_kings(BitBoard captured_pieces, Int2Type<true>) const
{
        return captured_pieces & king_targets_;
}

template<typename Rules, typename Board> template<bool Color>
bool State<Rules, Board>::is_promotion(BitBoard dest_sq) const
{
        return !Bit::is_zero(promotions<Color>(dest_sq));
}

template<typename Rules, typename Board>
bool State<Rules, Board>::is_promotion(void) const
{
        return current_.promotion;
}

template<typename Rules, typename Board>
bool State<Rules, Board>::current_greater_equal_best(void) const
{
        return greater_equal<Rules>()(current_, best_);
}

template<typename Rules, typename Board>
bool State<Rules, Board>::current_not_equal_to_best(void) const
{
        return not_equal_to<Rules>()(current_, best_);
}

template<typename Rules, typename Board>
void State<Rules, Board>::toggle_king_targets(void)
{
        initial_targets_ = remaining_targets_ ^= king_targets_;
}

template<typename Rules, typename Board>
void State<Rules, Board>::toggle_with_king(void)
{
        current_.with_king ^= true;
}

template<typename Rules, typename Board>
void State<Rules, Board>::toggle_promotion(void)
{
        current_.promotion ^= true;
}

template<typename Rules, typename Board>
void State<Rules, Board>::launch(BitBoard jump_sq)
{
        from_sq_ = jump_sq;
        not_occupied_ ^= jump_sq;
}

template<typename Rules, typename Board>
void State<Rules, Board>::finish(BitBoard jump_sq)
{
        not_occupied_ ^= jump_sq;
}

// tag dispatching on capture removal
template<typename Rules, typename Board>
void State<Rules, Board>::make(BitBoard target_sq)
{
        make(target_sq, Int2Type<Variants::CaptureRemoval<Rules>::VALUE>());
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Rules, typename Board>
void State<Rules, Board>::make(BitBoard target_sq, Int2Type<Variants::REMOVE_1>)
{
        not_occupied_ ^= target_sq;
        make(target_sq, Int2Type<Variants::REMOVE_N>());
}

// partial specialization for complete removal after a capture sequence
template<typename Rules, typename Board>
void State<Rules, Board>::make(BitBoard target_sq, Int2Type<Variants::REMOVE_N>)
{
        remaining_targets_ ^= target_sq;
        increment<Rules>()(current_, target_sq, king_targets_);
}

// tag dispatching on capture removal
template<typename Rules, typename Board>
void State<Rules, Board>::undo(BitBoard target_sq)
{
        undo(target_sq, Int2Type<Variants::CaptureRemoval<Rules>::VALUE>());
}

// partial specialization for piece by piece removal during a capture sequence
template<typename Rules, typename Board>
void State<Rules, Board>::undo(BitBoard target_sq, Int2Type<Variants::REMOVE_1>)
{
        undo(target_sq, Int2Type<Variants::REMOVE_N>());
        not_occupied_ ^= target_sq;
}

// partial specialization for complete removal after a capture sequence
template<typename Rules, typename Board>
void State<Rules, Board>::undo(BitBoard target_sq, Int2Type<Variants::REMOVE_N>)
{
        decrement<Rules>()(current_, target_sq, king_targets_);
        remaining_targets_ ^= target_sq;
}

template<typename Rules, typename Board>
void State<Rules, Board>::improve_best(Move::List& move_list)
{
        copy<Rules>()(best_, current_);
        move_list.clear();
}

// tag dispatching based on ambiguity of man captures
template<typename Rules, typename Board> template<bool Color>
void State<Rules, Board>::add_man_capture(BitBoard dest_sq, Move::List& move_list)
{
        add_man_capture<Color>(dest_sq, move_list, Int2Type<Variants::is_AmbiguousManCapture<Rules>::VALUE>());
}

// partial specialization for man captures that are unambiguous
template<typename Rules, typename Board> template<bool Color>
void State<Rules, Board>::add_man_capture(BitBoard dest_sq, Move::List& move_list, Int2Type<false>)
{
        const BitBoard captured_pieces = captured_targets();
        move_list.push_back<Color, Rules>(from_sq_ ^ dest_sq, promotions<Color>(dest_sq), captured_pieces, captured_kings(captured_pieces));
}

// partial specialization for man captures that can be ambiguous
template<typename Rules, typename Board> template<bool Color>
void State<Rules, Board>::add_man_capture(BitBoard dest_sq, Move::List& move_list, Int2Type<true>)
{
        const BitBoard captured_pieces = captured_targets();
        const bool ambiguous = !move_list.empty() && large<Rules>()(current_, captured_pieces);
        move_list.push_back<Color, Rules>(from_sq_ ^ dest_sq, promotions<Color>(dest_sq), captured_pieces, captured_kings(captured_pieces));
        if (ambiguous && move_list.non_unique_back<Rules>())
                move_list.pop_back();
}

// tag dispatching based on king halt after final capture
template<typename Rules, typename Board> template<bool Color, size_t Index>
void State<Rules, Board>::add_king_capture(BitBoard dest_sq, Move::List& move_list)
{
        add_king_capture<Color, Index>(dest_sq, move_list, Int2Type<Variants::KingCaptureHalt<Rules>::VALUE>());
}

// partial specialization for kings that halt immediately if the final capture is a king, and slide through otherwise
template<typename Rules, typename Board> template<bool Color, size_t Index>
void State<Rules, Board>::add_king_capture(BitBoard dest_sq, Move::List& move_list, Int2Type<Variants::HALT_K>)
{
        if (king_targets_ & Pull<Board, Index>()(dest_sq))
                add_king_capture<Color, Index>(dest_sq, move_list, Int2Type<Variants::HALT_1>());
        else
                add_king_capture<Color, Index>(dest_sq, move_list, Int2Type<Variants::HALT_N>());
}

// partial specialization for kings that halt immediately after the final capture
template<typename Rules, typename Board> template<bool Color, size_t Index>
void State<Rules, Board>::add_king_capture(BitBoard dest_sq, Move::List& move_list, Int2Type<Variants::HALT_1>)
{
        const BitBoard captured_pieces = captured_targets();
        const bool ambiguous = !move_list.empty() && large<Rules>()(current_, captured_pieces);
        add_king_capture<Color>(dest_sq, captured_pieces, captured_pieces & king_targets_, ambiguous, move_list);
}

// partial specialization for kings that slide through after the final capture
template<typename Rules, typename Board> template<bool Color, size_t Index>
void State<Rules, Board>::add_king_capture(BitBoard dest_sq, Move::List& move_list, Int2Type<Variants::HALT_N>)
{
        assert(dest_sq & path());

        const BitBoard captured_pieces = captured_targets();
        const BitBoard captured_kings = captured_pieces & king_targets_;
        const bool ambiguous = !move_list.empty() && large<Rules>()(current_, captured_pieces);
        do {
                add_king_capture<Color>(dest_sq, captured_pieces, captured_kings, ambiguous, move_list);
                PushAssign<Board, Index>()(dest_sq);
        } while (dest_sq & path());
}

// tag dispatching based on promotion condition
template<typename Rules, typename Board> template<bool Color>
void State<Rules, Board>::add_king_capture(BitBoard dest_sq, BitBoard captured_pieces, BitBoard captured_kings, bool ambiguous, Move::List& move_list)
{
        add_king_capture<Color>(dest_sq, captured_pieces, captured_kings, move_list, Int2Type<Variants::PromotionCondition<Rules>::VALUE>());
        if (ambiguous && move_list.non_unique_back<Rules>())
                move_list.pop_back();
}

// partial specialization for men that promote on the back row
template<typename Rules, typename Board> template<bool Color>
void State<Rules, Board>::add_king_capture(BitBoard dest_sq, BitBoard captured_pieces, BitBoard captured_kings, Move::List& move_list, Int2Type<Variants::PROMOTE_BR>)
{
        move_list.push_back<Color, Rules>(from_sq_ ^ dest_sq, captured_pieces, captured_kings);
}

// partial specialization for men that promote en-passant
template<typename Rules, typename Board> template<bool Color>
void State<Rules, Board>::add_king_capture(BitBoard dest_sq, BitBoard captured_pieces, BitBoard captured_kings, Move::List& move_list, Int2Type<Variants::PROMOTE_EP>)
{
        if (!is_promotion())
                move_list.push_back<Color, Rules>(from_sq_ ^ dest_sq,          captured_pieces, captured_kings);
        else
                move_list.push_back<Color, Rules>(from_sq_ ^ dest_sq, dest_sq, captured_pieces, captured_kings);
}

}        // namespace Capture
