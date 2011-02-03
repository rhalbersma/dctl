#include "Move.h"
#include "../Utilities/Bit.h"
#include <algorithm>

size_t MoveList::size(void) const
{
        return move_list_.size();
}

const Pieces& MoveList::operator[](size_t i) const
{
        return move_list_[i];
}

bool MoveList::empty(void) const
{
        return move_list_.empty();
}

// specialization for move generation without duplicate capture checking
bool MoveList::non_unique_back(Int2Type<false>)
{
        return false;
}

// specialization for move generation without duplicate capture checking
bool MoveList::non_unique_back(Int2Type<true>)
{
        return std::find(move_list_.begin(), move_list_.end(), move_list_.back()) != (move_list_.end() - 1);
}

// specialization for men that promote on the back row
bool MoveList::promotion_en_passant(BitBoard, BitBoard, Int2Type<Variant::PROMOTE_BR>)
{
        return false;
}

// specialization for men that promote en-passant
bool MoveList::promotion_en_passant(BitBoard promotion, BitBoard delta, Int2Type<Variant::PROMOTE_EP>)
{
        return Bit::is_single(promotion) && Bit::is_zero(delta);
}

// specialization for complete removal after a capture sequence
bool MoveList::sequential_capture_removal(BitBoard, BitBoard, Int2Type<Variant::REMOVE_N>)
{
        return false; 
}

// specialization for piece by piece removal during a capture sequence
bool MoveList::sequential_capture_removal(BitBoard delta, BitBoard captured_pieces, Int2Type<Variant::REMOVE_1>)
{
        return Bit::is_single(delta & captured_pieces) && Bit::is_multiple(captured_pieces); 
}

void MoveList::pop_back(void)
{
        return move_list_.pop_back();
}

void MoveList::clear(void)
{
        return move_list_.clear();
}
