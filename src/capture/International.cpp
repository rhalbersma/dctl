#include "International.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

Value<rules::International>::Value()
:
        num_pieces_(0)
{
}
        
bool Value<rules::International>::operator<(const Value<rules::International>& other) const
{
        return num_pieces_ < other.num_pieces_;
}

bool Value<rules::International>::operator==(const Value<rules::International>& other) const
{
        return num_pieces_ == other.num_pieces_;
}

bool Value<rules::International>::do_is_large(BitBoard) const
{
        return num_pieces_ >= rules::large_capture<rules::International>::value; 
}

void Value<rules::International>::do_increment(BitBoard, BitBoard)
{
        ++num_pieces_;
}

void Value<rules::International>::do_decrement(BitBoard, BitBoard)
{
        --num_pieces_;
}
        
}       // namespace capture
}       // namespace dctl
