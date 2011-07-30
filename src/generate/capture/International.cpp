#include "International.h"
#include "../../rules/Rules.h"

namespace dctl {
namespace generate {
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

void Value<rules::International>::increment(BitBoard, BitBoard)
{
        ++num_pieces;
}

void Value<rules::International>::decrement(BitBoard, BitBoard)
{
        --num_pieces;
}
        
template<>
bool operator<(const Value<rules::International>& left, const Value<rules::International>& right)
{
        return left.num_pieces < right.num_pieces;
}

template<>
bool operator==(const Value<rules::International>& left, const Value<rules::International>& right)
{
        return left.num_pieces == right.num_pieces;
}

}       // namespace capture
}       // namespace generate
}       // namespace dctl
