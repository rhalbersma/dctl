#include "BackAcknowledge.hpp"
#include "Parser.hpp"
#include "DXP.h"

namespace dctl {
namespace dxp {

const std::string BackAcknowledge::HEADER_ = "K";

const bool BackAcknowledge::REGISTERED_ = Parser<protocol>::register_message(HEADER_, create);

}       // namespace dxp
}       // namespace dctl
