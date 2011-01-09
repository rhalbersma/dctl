#include "DXP_BackAcknowledge.h"
#include "DXP_MessageFactory.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

const std::string DXP_BackAcknowledge::HEADER = "K";

const bool DXP_BackAcknowledge::REGISTERED = DXP_MessageFactory::register_creator(HEADER, create);

DXP_AbstractMessage* DXP_BackAcknowledge::create(const DXP_StringMessage& s)
{
        assert(s.header() == HEADER);
        return new DXP_BackAcknowledge(s.body());
}

DXP_BackAcknowledge::DXP_BackAcknowledge(const std::string& s)
:
        acceptance_code_(static_cast<AcceptanceCode>(atoi(s.substr(0, 1).c_str())))
{
}

DXP_BackAcknowledge::DXP_BackAcknowledge(AcceptanceCode a)
:
        acceptance_code_(a)
{
}

DXP_StringMessage DXP_BackAcknowledge::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << acceptance_code();

        return DXP_StringMessage(HEADER, sstr.str());
}

DXP_BackAcknowledge::AcceptanceCode DXP_BackAcknowledge::acceptance_code(void) const
{
        return acceptance_code_;
}
