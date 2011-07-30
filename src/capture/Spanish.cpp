#include "Spanish.h"
#include "../rules/Rules.h"

namespace dctl {
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
        
// http://fpdamas.home.sapo.pt/regrasclass.htm
bool Value<rules::Spanish>::operator<(const Value<rules::Spanish>& other) const
{
        // 15 b)
        if (num_pieces < other.num_pieces)
                return true;
        if (num_pieces > other.num_pieces)
                return false;

        // 15 c)
        return num_kings < other.num_kings;
}

bool Value<rules::Spanish>::operator==(const Value<rules::Spanish>& other) const
{
        return (
                (num_pieces == other.num_pieces) &&
                 (num_kings == other.num_kings)
        );
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

}       // namespace capture
}       // namespace dctl
