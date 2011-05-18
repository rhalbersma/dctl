#include "GameEnd.h"
#include "Parser.h"
#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace DXP = DamExchange;

const std::string DXP::Layer2::GameEnd::HEADER = "E";

const bool DXP::Layer2::GameEnd::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<DXP::Layer2::AbstractMessage> DXP::Layer2::GameEnd::create(const std::string& msg)
{
        return std::make_shared<GameEnd>(msg);
}

DXP::Layer2::GameEnd::GameEnd(const std::string& msg)
:
        reason_(static_cast<Reason>(boost::lexical_cast<size_t>(msg.substr(0, 1).c_str()))),
        stop_code_(static_cast<StopCode>(boost::lexical_cast<size_t>(msg.substr(1, 1).c_str())))
{
}

DXP::Layer2::GameEnd::GameEnd(Reason r, StopCode s)
:
        reason_(r),
        stop_code_(s)
{
}

DXP::Layer2::GameEnd::Reason DXP::Layer2::GameEnd::reason(void) const
{
        return reason_;
}

DXP::Layer2::GameEnd::StopCode DXP::Layer2::GameEnd::stop_code(void) const
{
        return stop_code_;
}

std::string DXP::Layer2::GameEnd::header(void) const
{
        return HEADER;
}

std::string DXP::Layer2::GameEnd::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw( 1) << reason();
        sstr << std::setw( 1) << stop_code();
        return sstr.str();
}
