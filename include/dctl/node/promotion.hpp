#pragma once
#include <dctl/board/mask/promotion.hpp>

namespace dctl {

template<bool Color, class Board>
bool is_promotion(int sq)
{
        return board::Promotion<Board>::mask(Color).test(sq);
}

}       // namespace dctl
