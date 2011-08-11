#include "Russian.h"
#include "../rules/Rules.h"
#include "../bit/Bit.h"

namespace dctl {
namespace capture {

Value<rules::Russian>::Value()
:
        promotion_(false)
{
}

bool Value<rules::Russian>::do_is_large(BitBoard captured_pieces) const
{
        return bit::count(captured_pieces) >= rules::large_capture<rules::Russian>::value;
}

bool Value<rules::Russian>::do_is_promotion() const
{
        return promotion_;
}

void Value<rules::Russian>::do_toggle_promotion()
{
        promotion_ ^= TOGGLE;
} 

}       // namespace capture
}       // namespace dctl
