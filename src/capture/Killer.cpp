#include "Killer.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

Value<rules::Killer>::Value()
:
        num_pieces_(0)
{
}
        
bool Value<rules::Killer>::operator<(const Value<rules::Killer>& other) const
{
        return num_pieces_ < other.num_pieces_;
}

bool Value<rules::Killer>::operator==(const Value<rules::Killer>& other) const
{
        return num_pieces_ == other.num_pieces_;
}

bool Value<rules::Killer>::do_is_large(BitBoard) const
{
        return num_pieces_ >= rules::large_capture<rules::Killer>::value; 
}

void Value<rules::Killer>::do_increment(BitBoard, BitBoard)
{
        ++num_pieces_;
}

void Value<rules::Killer>::do_decrement(BitBoard, BitBoard)
{
        --num_pieces_;
}

}       // namespace capture
}       // namespace dctl
