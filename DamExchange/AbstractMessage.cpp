#include "AbstractMessage.h"

namespace DXP = DamExchangeProtocol;

std::string DXP::AbstractMessage::str(void) const
{
        return header() + body();
}
