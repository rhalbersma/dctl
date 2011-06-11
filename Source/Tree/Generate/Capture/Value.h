#pragma once
#include "../../../Utilities/IntegerTypes.h"

namespace tree {
namespace generate {
namespace capture {

struct Value
{
        BitBoard piece_order;
        PieceCount num_pieces;
        PieceCount num_kings;
        bool with_king;
        bool promotion;
};

}       // namespace capture
}       // namespace generate
}       // namespace tree
