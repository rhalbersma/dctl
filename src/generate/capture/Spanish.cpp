#include "Spanish.h"
#include "../../rules/Rules.h"

namespace dctl {
namespace generate {
namespace capture {

Value<rules::Spanish>::Value()
:
        num_pieces(0),
        num_kings(0)
{
}
        
bool Value<rules::Spanish>::is_large(BitBoard) const
{
        return num_pieces >= rules::large_capture<rules::Spanish>::value; 
}
        
void Value<rules::Spanish>::increment(BitBoard target_sq, BitBoard king_targets)
{
        ++num_pieces;
        if (target_sq & king_targets)
                ++num_kings;
}

void Value<rules::Spanish>::decrement(BitBoard target_sq, BitBoard king_targets)
{
        if (target_sq & king_targets)
                --num_kings;
        --num_pieces;
}
        
// http://fpdamas.home.sapo.pt/regrasclass.htm
template<>
bool operator<(const Value<rules::Spanish>& left, const Value<rules::Spanish>& right)
{
        // 15 b)
        if (left.num_pieces < right.num_pieces)
                return true;
        if (left.num_pieces > right.num_pieces)
                return false;

        // 15 c)
        return left.num_kings < right.num_kings;
}

template<>
bool operator==(const Value<rules::Spanish>& left, const Value<rules::Spanish>& right)
{
        return (
                (left.num_pieces == right.num_pieces) &&
                 (left.num_kings == right.num_kings)
        );
}

}       // namespace capture
}       // namespace generate
}       // namespace dctl
