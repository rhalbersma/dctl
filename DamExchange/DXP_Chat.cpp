#include "DXP_Chat.h"
#include <iomanip>
#include <sstream>

DXP_Chat::DXP_Chat(const std::string& m)
:
        message(m)
{
}

std::string DXP_Chat::send_message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << message;

        return sstr.str();
}
