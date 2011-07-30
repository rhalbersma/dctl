#include "Killer.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

Value<rules::Killer>::Value()
:
        num_pieces(0)
{
}
        
bool Value<rules::Killer>::is_large(BitBoard) const
{
        return num_pieces >= rules::large_capture<rules::Killer>::value; 
}

bool Value<rules::Killer>::operator<(const Value<rules::Killer>& other) const
{
        return num_pieces < other.num_pieces;
}

bool Value<rules::Killer>::operator==(const Value<rules::Killer>& other) const
{
        return num_pieces == other.num_pieces;
}

void Value<rules::Killer>::increment(BitBoard, BitBoard)
{
        ++num_pieces;
}

void Value<rules::Killer>::decrement(BitBoard, BitBoard)
{
        --num_pieces;
}

}       // namespace capture
}       // namespace dctl
