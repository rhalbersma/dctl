#include "GameAcknowledge.h"
#include "Parser.h"
#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace DXP = DamExchange;

const std::string DXP::Layer2::GameAcknowledge::HEADER = "A";

const bool DXP::Layer2::GameAcknowledge::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<DXP::Layer2::AbstractMessage> DXP::Layer2::GameAcknowledge::create(const std::string& msg)
{
        return std::make_shared<GameAcknowledge>(msg);
}

DXP::Layer2::GameAcknowledge::GameAcknowledge(const std::string& msg)
:
        name_follower_(msg.substr(0, 32)),
        acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<size_t>(msg.substr(32, 1).c_str())))
{
}

DXP::Layer2::GameAcknowledge::GameAcknowledge(const std::string& n, AcceptanceCode a)
:
        name_follower_(n),
        acceptance_code_(a)
{
}

const std::string& DXP::Layer2::GameAcknowledge::name_follower(void) const
{
        return name_follower_;
}

DXP::Layer2::GameAcknowledge::AcceptanceCode DXP::Layer2::GameAcknowledge::acceptance_code(void) const
{
        return acceptance_code_;
}

std::string DXP::Layer2::GameAcknowledge::header(void) const
{
        return HEADER;
}

std::string DXP::Layer2::GameAcknowledge::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw(32) << name_follower() << std::setfill(' ');
        sstr << std::setw( 1) << acceptance_code();
        return sstr.str();
}
