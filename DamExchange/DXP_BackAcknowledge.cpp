#include "DXP_BackAcknowledge.h"
#include <iomanip>
#include <sstream>

DXP_BackAcknowledge::DXP_BackAcknowledge(const std::string&)
{
}

std::string DXP_BackAcknowledge::send_message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw( 1) << acceptance_code;

        return sstr.str();
}
