#pragma once
#include "../Utilities/IntegerTypes.h"

struct CaptureValue
{
        BitBoard piece_order;
        size_t num_pieces;
        size_t num_kings;
        bool with_king;
        bool promotion;
};
