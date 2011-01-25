#include "AbstractMessage.h"

namespace DXP = DamExchangeProtocol;

DXP::StringMessage DXP::AbstractMessage::message(void) const
{
        return StringMessage(header() + body());
}
