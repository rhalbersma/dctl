#include "Predicates.h"
#include "../../utils/Bit.h"

namespace tree {

// specialization for piece by piece removal during a capture sequence (Thai draughts)
bool is_intersecting_capture(BitBoard delta, BitBoard captured_pieces, Int2Type<rules::REMOVE_1>)
{
        // [FEN "W:WK26:B9,12,18,19"] white has to capture 26x12, landing on a square it also captured on
        return bit::is_single(delta & captured_pieces) && bit::is_multiple(captured_pieces); 
}

// specialization for complete removal after a capture sequence
bool is_intersecting_capture(BitBoard, BitBoard, Int2Type<rules::REMOVE_N>)
{
        return false; 
}

// specialization for men that promote on the back row
bool is_intersecting_promotion(BitBoard, BitBoard, Int2Type<rules::PROMOTE_BR>)
{
        return false;
}

// specialization for men that promote en-passant (Russian draughts)
bool is_intersecting_promotion(BitBoard promotion, BitBoard delta, Int2Type<rules::PROMOTE_EP>)
{
        // [FEN "W:W15:B10,13,20,23"] white has to capture 15x15, promoting on its original square
        return bit::is_single(promotion) && bit::is_zero(delta);
}

}       // namespace tree
