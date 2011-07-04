#include "MessageInterface.h"

namespace DXP = damexchange;

std::string DXP::layer2::MessageInterface::str(void) const
{
        return header() + body();
}
