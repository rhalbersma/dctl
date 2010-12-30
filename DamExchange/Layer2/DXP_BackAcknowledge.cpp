#include "DXP_BackAcknowledge.h"
#include "DXP_MessageFactory.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

const bool DXP_BackAcknowledge::REGISTERED = DXP_MessageFactory::register_creator(HEADER, create);

DXP_AbstractMessage* DXP_BackAcknowledge::create(const DXP_String& s)
{
        assert(s.header() == HEADER);
        return new DXP_BackAcknowledge(s.content());
}

DXP_BackAcknowledge::DXP_BackAcknowledge(const std::string& s)
:
        d_acceptance_code(static_cast<AcceptanceCode>(atoi(s.substr(0, 1).c_str())))
{
}

DXP_BackAcknowledge::DXP_BackAcknowledge(AcceptanceCode a)
:
        d_acceptance_code(a)
{
}

DXP_String DXP_BackAcknowledge::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << acceptance_code();

        return DXP_String(HEADER, sstr.str());
}

DXP_BackAcknowledge::AcceptanceCode DXP_BackAcknowledge::acceptance_code(void) const
{
        return d_acceptance_code;
}
