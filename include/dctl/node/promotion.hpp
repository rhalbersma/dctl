#pragma once

namespace dctl {

template<bool Color, class Board>
bool is_promotion(int sq)
{
        return Board::promotion_mask[Color][0].test(sq);
}

}       // namespace dctl
