#include "ValueInterface.h"

namespace dctl {
namespace capture {

bool ValueInterface::is_large(BitBoard captured_pieces) const
{
        return do_is_large(captured_pieces);
}

bool ValueInterface::is_promotion() const
{
        return do_is_promotion();
}

void ValueInterface::increment(BitBoard target_sq, BitBoard king_targets)
{
        do_increment(target_sq, king_targets);
}

void ValueInterface::decrement(BitBoard target_sq, BitBoard king_targets)
{
        do_decrement(target_sq, king_targets);
}

void ValueInterface::toggle_with_king()
{
        do_toggle_with_king();
}

void ValueInterface::toggle_promotion()
{
        do_toggle_promotion();
}

}       // namespace capture
}       // namespace dctl
