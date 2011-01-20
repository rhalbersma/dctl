#include "AbstractMessage.h"

namespace DXP  = DamExchangeProtocol;

DXP::StringMessage DXP::AbstractMessage::message(void) const
{
        return DXP::StringMessage(header(), body());
}
