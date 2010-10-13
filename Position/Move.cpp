#include "Move.h"
#include "../Utilities/Bit.h"
#include <algorithm>

bool Move::unique_back(const List& v)
{
        return std::find(v.begin(), &v.back(), v.back()) == &v.back();
}

bool Move::promotion_en_passant(BitBoard promotion, BitBoard delta)
{
        return Bit::is_single(promotion) && Bit::is_zero(delta);
}

bool Move::coup_thai(BitBoard delta, BitBoard captured_pieces)
{
        return Bit::is_single(delta & captured_pieces) && Bit::is_multiple(captured_pieces); 
}
