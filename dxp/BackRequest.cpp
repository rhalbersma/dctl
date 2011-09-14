#include "BackRequest.hpp"
#include "Parser.hpp"
#include "DXP.h"

namespace dctl {
namespace dxp {

const std::string BackRequest::HEADER_ = "B";

const bool BackRequest::REGISTERED_ = Parser<protocol>::register_message(HEADER_, create);

}       // namespace dxp
}       // namespace dctl
