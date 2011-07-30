#include "Killer.h"
#include "../../rules/Rules.h"

namespace dctl {
namespace generate {
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

void Value<rules::Killer>::increment(BitBoard, BitBoard)
{
        ++num_pieces;
}

void Value<rules::Killer>::decrement(BitBoard, BitBoard)
{
        --num_pieces;
}

template<>
bool operator<(const Value<rules::Killer>& left, const Value<rules::Killer>& right)
{
        return left.num_pieces < right.num_pieces;
}

template<>
bool operator==(const Value<rules::Killer>& left, const Value<rules::Killer>& right)
{
        return left.num_pieces == right.num_pieces;
}

}       // namespace capture
}       // namespace generate
}       // namespace dctl
