#include "Russian.h"
#include "../rules/Rules.h"
#include "../bit/Bit.h"

namespace dctl {
namespace capture {

Value<variant::Russian>::Value()
:
        promotion_(false)
{
}

bool Value<variant::Russian>::do_is_large(BitBoard captured_pieces) const
{
        return bit::count(captured_pieces) >= rules::large_capture<variant::Russian>::value;
}

bool Value<variant::Russian>::do_is_promotion() const
{
        return promotion_;
}

void Value<variant::Russian>::do_toggle_promotion()
{
        promotion_ ^= TOGGLE;
} 

}       // namespace capture
}       // namespace dctl
