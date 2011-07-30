#include "Russian.h"
#include "../../rules/Rules.h"

namespace dctl {
namespace generate {
namespace capture {

Value<rules::Russian>::Value()
:
        promotion(false)
{
}

bool Value<rules::Russian>::is_large(BitBoard captured_pieces) const
{
        return bit::count(captured_pieces) >= rules::large_capture<rules::Russian>::value;
}

}       // namespace capture
}       // namespace generate
}       // namespace dctl
