#include "Killer.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

Value<variant::Killer>::Value()
:
        num_pieces_(0)
{
}
        
bool Value<variant::Killer>::operator<(const Value<variant::Killer>& other) const
{
        return num_pieces_ < other.num_pieces_;
}

bool Value<variant::Killer>::operator==(const Value<variant::Killer>& other) const
{
        return num_pieces_ == other.num_pieces_;
}

bool Value<variant::Killer>::do_is_large(BitBoard) const
{
        return num_pieces_ >= rules::large_capture<variant::Killer>::value; 
}

void Value<variant::Killer>::do_increment(BitBoard, BitBoard)
{
        ++num_pieces_;
}

void Value<variant::Killer>::do_decrement(BitBoard, BitBoard)
{
        --num_pieces_;
}

}       // namespace capture
}       // namespace dctl
