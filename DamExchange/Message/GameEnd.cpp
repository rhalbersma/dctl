#include "GameEnd.h"
#include "Parser.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace DXP = DamExchangeProtocol;
namespace Layer2 = DXP::Message;

const std::string Layer2::GameEnd::HEADER = "E";

const bool Layer2::GameEnd::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<Layer2::AbstractMessage> Layer2::GameEnd::create(const std::string& s)
{
        return std::make_shared<GameEnd>(s);
}

Layer2::GameEnd::GameEnd(const std::string& s)
:
        reason_(static_cast<Reason>(atoi(s.substr(0, 1).c_str()))),
        stop_code_(static_cast<StopCode>(atoi(s.substr(1, 1).c_str())))
{
}

Layer2::GameEnd::GameEnd(Reason r, StopCode s)
:
        reason_(r),
        stop_code_(s)
{
}

Layer2::GameEnd::Reason Layer2::GameEnd::reason(void) const
{
        return reason_;
}

Layer2::GameEnd::StopCode Layer2::GameEnd::stop_code(void) const
{
        return stop_code_;
}

std::string Layer2::GameEnd::header(void) const
{
        return HEADER;
}

std::string Layer2::GameEnd::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw( 1) << reason();
        sstr << std::setw( 1) << stop_code();
        return sstr.str();
}
