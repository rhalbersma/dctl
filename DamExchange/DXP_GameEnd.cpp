#include "DXP_GameEnd.h"
#include <iomanip>
#include <sstream>

DXP_GameEnd::DXP_GameEnd(const std::string&)
{
}

std::string DXP_GameEnd::send_message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw( 1) << reason;
        sstr << std::setw( 1) << stop_code;

        return sstr.str();
}
