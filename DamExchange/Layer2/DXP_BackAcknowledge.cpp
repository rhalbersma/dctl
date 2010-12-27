#include "DXP_BackAcknowledge.h"
#include <cstdlib>
#include <iomanip>
#include <sstream>

DXP_BackAcknowledge::DXP_BackAcknowledge(AcceptanceCode a)
:
        d_acceptance_code(a)
{
}

DXP_BackAcknowledge::DXP_BackAcknowledge(const std::string& s)
:
        d_acceptance_code(static_cast<AcceptanceCode>(atoi(s.substr(1, 1).c_str())))
{
}

std::string DXP_BackAcknowledge::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw( 1) << acceptance_code();

        return sstr.str();
}

DXP_BackAcknowledge::AcceptanceCode DXP_BackAcknowledge::acceptance_code(void) const
{
        return d_acceptance_code;
}
