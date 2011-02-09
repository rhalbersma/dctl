#include "GameEnd.h"
#include "Factory.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace DXP = DamExchangeProtocol;

const std::string DXP::GameEnd::HEADER = "E";

const bool DXP::GameEnd::REGISTERED = Factory::register_creator(HEADER, create);

std::shared_ptr<DXP::AbstractMessage> DXP::GameEnd::create(const std::string& s)
{
        return std::make_shared<GameEnd>(s);
}

DXP::GameEnd::GameEnd(const std::string& s)
:
        reason_(static_cast<Reason>(atoi(s.substr(0, 1).c_str()))),
        stop_code_(static_cast<StopCode>(atoi(s.substr(1, 1).c_str())))
{
}

DXP::GameEnd::GameEnd(Reason r, StopCode s)
:
        reason_(r),
        stop_code_(s)
{
}

DXP::GameEnd::Reason DXP::GameEnd::reason(void) const
{
        return reason_;
}

DXP::GameEnd::StopCode DXP::GameEnd::stop_code(void) const
{
        return stop_code_;
}

std::string DXP::GameEnd::header(void) const
{
        return HEADER;
}

std::string DXP::GameEnd::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw( 1) << reason();
        sstr << std::setw( 1) << stop_code();
        return sstr.str();
}
