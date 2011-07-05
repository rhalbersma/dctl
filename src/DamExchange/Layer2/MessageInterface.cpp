#include "MessageInterface.h"

namespace damexchange {
namespace layer2 {

std::string MessageInterface::str() const
{
        return header() + body();
}

}       // namespace layer2
}       // namespace damexchange
