#include "MessageInterface.h"

namespace dctl {
namespace protocol {
namespace damexchange {

std::string MessageInterface::str() const
{
        return header() + body();
}

}       // namespace damexchange
}       // namespace protocol
}       // namespace dctl
