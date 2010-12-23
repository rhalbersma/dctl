#include "DXP_GameAcknowledge.h"
#include <iomanip>
#include <sstream>

DXP_GameAcknowledge::DXP_GameAcknowledge(const std::string&)
{
}

std::string DXP_GameAcknowledge::send_message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw(32) << name_follower << std::setfill(' ');
        sstr << std::setw( 1) << acceptance_code;

        return sstr.str();
}
