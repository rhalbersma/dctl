#include "AbstractMessage.h"

namespace DXP = DamExchangeProtocol;
namespace Layer2 = DXP::Message;

std::string Layer2::AbstractMessage::str(void) const
{
        return header() + body();
}
