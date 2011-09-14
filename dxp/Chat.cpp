#include "Chat.hpp"
#include "Parser.hpp"
#include "DXP.h"

namespace dctl {
namespace dxp {

const std::string Chat::HEADER_ = "C";

const bool Chat::REGISTERED_ = Parser<protocol>::register_message(HEADER_, create);

}       // namespace dxp
}       // namespace dctl
