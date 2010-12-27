#include "DXP_GameEnd.h"
#include <cstdlib>
#include <iomanip>
#include <sstream>

DXP_GameEnd::DXP_GameEnd(size_t r, StopCode s)
:
        d_reason(r),
        d_stop_code(s)
{
}

DXP_GameEnd::DXP_GameEnd(const std::string& s)
:
        d_reason(atoi(s.substr(1, 1).c_str())),
        d_stop_code(static_cast<StopCode>(atoi(s.substr(2, 1).c_str())))
{
}

std::string DXP_GameEnd::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw( 1) << reason();
        sstr << std::setw( 1) << stop_code();

        return sstr.str();
}

size_t DXP_GameEnd::reason(void) const
{
        return d_reason;
}

DXP_GameEnd::StopCode DXP_GameEnd::stop_code(void) const
{
        return d_stop_code;
}
