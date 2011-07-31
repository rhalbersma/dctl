#include "Spanish.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

Value<rules::Spanish>::Value()
:
        num_pieces_(0),
        num_kings_(0)
{
}
        
bool Value<rules::Spanish>::is_large(BitBoard) const
{
        return num_pieces_ >= rules::large_capture<rules::Spanish>::value; 
}
        
// http://fpdamas.home.sapo.pt/regrasclass.htm
bool Value<rules::Spanish>::operator<(const Value<rules::Spanish>& other) const
{
        // 15 b)
        if (num_pieces_ < other.num_pieces_)
                return true;
        if (num_pieces_ > other.num_pieces_)
                return false;

        // 15 c)
        return num_kings_ < other.num_kings_;
}

bool Value<rules::Spanish>::operator==(const Value<rules::Spanish>& other) const
{
        return (
                (num_pieces_ == other.num_pieces_) &&
                 (num_kings_ == other.num_kings_)
        );
}

void Value<rules::Spanish>::increment(BitBoard target_sq, BitBoard king_targets)
{
        ++num_pieces_;
        if (target_sq & king_targets)
                ++num_kings_;
}

void Value<rules::Spanish>::decrement(BitBoard target_sq, BitBoard king_targets)
{
        if (target_sq & king_targets)
                --num_kings_;
        --num_pieces_;
}

}       // namespace capture
}       // namespace dctl
