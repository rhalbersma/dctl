#include "DXP_GameAcknowledge.h"
#include "DXP_MessageFactory.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

const std::string DXP_GameAcknowledge::HEADER = "A";

const bool DXP_GameAcknowledge::REGISTERED = DXP_MessageFactory::register_creator(HEADER, create);

DXP_AbstractMessage* DXP_GameAcknowledge::create(const DXP_StringMessage& s)
{
        assert(s.header() == HEADER);
        return new DXP_GameAcknowledge(s.body());
}

DXP_GameAcknowledge::DXP_GameAcknowledge(const std::string& s)
:
        name_follower_(s.substr(0, 32)),
        acceptance_code_(static_cast<AcceptanceCode>(atoi(s.substr(32, 1).c_str())))
{
}

DXP_GameAcknowledge::DXP_GameAcknowledge(const std::string& n, AcceptanceCode a)
:
        name_follower_(n),
        acceptance_code_(a)
{
}

DXP_StringMessage DXP_GameAcknowledge::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw(32) << name_follower() << std::setfill(' ');
        sstr << std::setw( 1) << acceptance_code();

        return DXP_StringMessage(HEADER, sstr.str());
}

const std::string& DXP_GameAcknowledge::name_follower(void) const
{
        return name_follower_;
}

DXP_GameAcknowledge::AcceptanceCode DXP_GameAcknowledge::acceptance_code(void) const
{
        return acceptance_code_;
}
