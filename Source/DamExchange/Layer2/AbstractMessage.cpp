#include "AbstractMessage.h"

namespace DXP = damexchange;

std::string DXP::layer2::AbstractMessage::str(void) const
{
        return header() + body();
}
