#include "International.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

Value<rules::International>::Value()
:
        num_pieces(0)
{
}
        
bool Value<rules::International>::is_large(BitBoard) const
{
        return num_pieces >= rules::large_capture<rules::International>::value; 
}

bool Value<rules::International>::operator<(const Value<rules::International>& other) const
{
        return num_pieces < other.num_pieces;
}

bool Value<rules::International>::operator==(const Value<rules::International>& other) const
{
        return num_pieces == other.num_pieces;
}

void Value<rules::International>::increment(BitBoard, BitBoard)
{
        ++num_pieces;
}

void Value<rules::International>::decrement(BitBoard, BitBoard)
{
        --num_pieces;
}
        
}       // namespace capture
}       // namespace dctl
