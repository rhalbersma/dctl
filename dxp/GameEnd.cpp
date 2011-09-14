#include "GameEnd.hpp"
#include "Parser.hpp"
#include "DXP.h"

namespace dctl {
namespace dxp {

const std::string GameEnd::HEADER_ = "E";

const bool GameEnd::REGISTERED_ = Parser<protocol>::register_message(HEADER_, create);

}       // namespace dxp
}       // namespace dctl
