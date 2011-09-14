#include <algorithm>    // std::for_each
#include "Move.hpp"
#include "Parser.hpp"
#include "DXP.h"

namespace dctl {
namespace dxp {

const std::string Move::HEADER_ = "M";

const bool Move::REGISTERED_ = Parser<protocol>::register_message(HEADER_, create);

}       // namespace dxp
}       // namespace dctl
