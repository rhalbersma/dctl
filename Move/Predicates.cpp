#include "Predicates.h"
#include "../Utilities/Bit.h"

// specialization for piece by piece removal during a capture sequence
bool Move::is_sequential_capture_removal(BitBoard delta, BitBoard captured_pieces, Int2Type<Variants::REMOVE_1>)
{
        return Bit::is_single(delta & captured_pieces) && Bit::is_multiple(captured_pieces); 
}

// specialization for complete removal after a capture sequence
bool Move::is_sequential_capture_removal(BitBoard, BitBoard, Int2Type<Variants::REMOVE_N>)
{
        return false; 
}

// specialization for men that promote on the back row
bool Move::is_promotion_en_passant(BitBoard, BitBoard, Int2Type<Variants::PROMOTE_BR>)
{
        return false;
}

// specialization for men that promote en-passant
bool Move::is_promotion_en_passant(BitBoard promotion, BitBoard delta, Int2Type<Variants::PROMOTE_EP>)
{
        return Bit::is_single(promotion) && Bit::is_zero(delta);
}
