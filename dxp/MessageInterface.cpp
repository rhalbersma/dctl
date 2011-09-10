#include "MessageInterface.h"

namespace dctl {
namespace dxp {

std::string MessageInterface::str() const
{
        return header() + body();
}

}       // namespace dxp
}       // namespace dctl
