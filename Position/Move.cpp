#include "Move.h"
#include "../Utilities/Bit.h"
#include <algorithm>

// specialization for move generation without duplicate capture checking
bool Move::non_unique_back(const List&, Int2Type<false>)
{
        return false;
}

// specialization for move generation without duplicate capture checking
bool Move::non_unique_back(const List& v, Int2Type<true>)
{
        return std::find(v.begin(), v.end(), v.back()) != (v.end() - 1);
}

// specialization for men that promote on the back row
bool Move::promotion_en_passant(BitBoard, BitBoard, Int2Type<PROMOTE_BR>)
{
        return false;
}

// specialization for men that promote en-passant
bool Move::promotion_en_passant(BitBoard promotion, BitBoard delta, Int2Type<PROMOTE_EP>)
{
        return Bit::is_single(promotion) && Bit::is_zero(delta);
}

// specialization for complete removal after a capture sequence
bool Move::sequential_capture_removal(BitBoard, BitBoard, Int2Type<REMOVE_N>)
{
        return false; 
}

// specialization for piece by piece removal during a capture sequence
bool Move::sequential_capture_removal(BitBoard delta, BitBoard captured_pieces, Int2Type<REMOVE_1>)
{
        return Bit::is_single(delta & captured_pieces) && Bit::is_multiple(captured_pieces); 
}
