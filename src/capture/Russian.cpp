#include "Russian.h"
#include "../rules/Rules.h"
#include "../utils/Bit.h"

namespace dctl {
namespace capture {

Value<rules::Russian>::Value()
:
        promotion_(false)
{
}

bool Value<rules::Russian>::is_large(BitBoard captured_pieces) const
{
        return bit::count(captured_pieces) >= rules::large_capture<rules::Russian>::value;
}

bool Value<rules::Russian>::is_promotion() const
{
        return promotion_;
}

void Value<rules::Russian>::toggle_promotion()
{
        promotion_ ^= TOGGLE;
} 

}       // namespace capture
}       // namespace dctl
