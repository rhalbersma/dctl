#include "MessageInterface.h"

namespace damexchange {

std::string MessageInterface::str() const
{
        return header() + body();
}

}       // namespace damexchange
