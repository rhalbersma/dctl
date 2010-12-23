#include "DXP_BackRequest.h"
#include <iomanip>
#include <sstream>

DXP_BackRequest::DXP_BackRequest(const std::string&)
{
}

std::string DXP_BackRequest::send_message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw( 3) << std::setfill('0') << move_number;
        sstr << std::setw( 1) << side_to_move;

        return sstr.str();
}
