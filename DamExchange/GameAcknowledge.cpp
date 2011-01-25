#include "GameAcknowledge.h"
#include "MessageFactory.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace DXP = DamExchangeProtocol;

const std::string DXP::GameAcknowledge::HEADER = "A";

const bool DXP::GameAcknowledge::REGISTERED = MessageFactory::register_creator(HEADER, create);

std::shared_ptr<DXP::AbstractMessage> DXP::GameAcknowledge::create(const StringMessage& s)
{
        assert(REGISTERED);
        assert(s.header() == HEADER);
        return std::make_shared<GameAcknowledge>(s.body());
}

DXP::GameAcknowledge::GameAcknowledge(const std::string& s)
:
        name_follower_(s.substr(0, 32)),
        acceptance_code_(static_cast<AcceptanceCode>(atoi(s.substr(32, 1).c_str())))
{
}

DXP::GameAcknowledge::GameAcknowledge(const std::string& n, AcceptanceCode a)
:
        name_follower_(n),
        acceptance_code_(a)
{
}

const std::string& DXP::GameAcknowledge::name_follower(void) const
{
        return name_follower_;
}

DXP::GameAcknowledge::AcceptanceCode DXP::GameAcknowledge::acceptance_code(void) const
{
        return acceptance_code_;
}

std::string DXP::GameAcknowledge::header(void) const
{
        return HEADER;
}

std::string DXP::GameAcknowledge::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw(32) << name_follower() << std::setfill(' ');
        sstr << std::setw( 1) << acceptance_code();
        return sstr.str();
}
