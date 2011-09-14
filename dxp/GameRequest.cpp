#include "GameRequest.hpp"
#include "Parser.hpp"
#include "DXP.h"

namespace dctl {
namespace dxp {

const std::string GameRequest::HEADER_ = "R";

const char GameRequest::SETUP[2] = {
        INITIAL,
        SPECIAL
};

const bool GameRequest::REGISTERED_ = Parser<protocol>::register_message(HEADER_, create);

}       // namespace dxp
}       // namespace dctl
