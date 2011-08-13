#include "International.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

Value<variant::International>::Value()
:
        num_pieces_(0)
{
}
        
bool Value<variant::International>::operator<(const Value<variant::International>& other) const
{
        return num_pieces_ < other.num_pieces_;
}

bool Value<variant::International>::operator==(const Value<variant::International>& other) const
{
        return num_pieces_ == other.num_pieces_;
}

bool Value<variant::International>::do_is_large(BitBoard) const
{
        return num_pieces_ >= rules::large_capture<variant::International>::value; 
}

void Value<variant::International>::do_increment(BitBoard, BitBoard)
{
        ++num_pieces_;
}

void Value<variant::International>::do_decrement(BitBoard, BitBoard)
{
        --num_pieces_;
}
        
}       // namespace capture
}       // namespace dctl
