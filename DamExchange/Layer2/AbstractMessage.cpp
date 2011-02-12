#include "AbstractMessage.h"

namespace DXP = DamExchange;

std::string DXP::Layer2::AbstractMessage::str(void) const
{
        return header() + body();
}
