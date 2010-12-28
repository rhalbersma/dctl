#include "DXP_Chat.h"
#include <cassert>
#include <iomanip>
#include <sstream>

DXP_Chat::DXP_Chat(const std::string& s)
:
        d_message(s.substr(1))
{
        assert(invariant(*s.begin()));
}

char DXP_Chat::header(void) const
{
        return HEADER;
}

std::string DXP_Chat::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << d_message;

        return sstr.str();
}
