#include "MessageInterface.h"

namespace dctl {
namespace protocol {
namespace dxp {

std::string MessageInterface::str() const
{
        return header() + body();
}

}       // namespace dxp
}       // namespace protocol
}       // namespace dctl
