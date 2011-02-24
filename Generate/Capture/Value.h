#pragma once
#include "../../Utilities/IntegerTypes.h"

namespace Capture {

struct Value
{
        BitBoard piece_order;
        PieceCount num_pieces;
        PieceCount num_kings;
        bool with_king;
        bool promotion;
};

}       // namespace Capture
