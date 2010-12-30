#include "DXP_GameAcknowledge.h"
#include "DXP_MessageFactory.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

const bool DXP_GameAcknowledge::REGISTERED = DXP_MessageFactory::register_creator(HEADER, create);

DXP_AbstractMessage* DXP_GameAcknowledge::create(const DXP_String& s)
{
        assert(s.header() == HEADER);
        return new DXP_GameAcknowledge(s.content());
}

DXP_GameAcknowledge::DXP_GameAcknowledge(const std::string& s)
:
        d_name_follower(s.substr(0, 32)),
        d_acceptance_code(static_cast<AcceptanceCode>(atoi(s.substr(32, 1).c_str())))
{
}

DXP_GameAcknowledge::DXP_GameAcknowledge(const std::string& n, AcceptanceCode a)
:
        d_name_follower(n),
        d_acceptance_code(a)
{
}

DXP_String DXP_GameAcknowledge::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw(32) << name_follower() << std::setfill(' ');
        sstr << std::setw( 1) << acceptance_code();

        return DXP_String(HEADER, sstr.str());
}

const std::string& DXP_GameAcknowledge::name_follower(void) const
{
        return d_name_follower;
}

DXP_GameAcknowledge::AcceptanceCode DXP_GameAcknowledge::acceptance_code(void) const
{
        return d_acceptance_code;
}
