#include "DXP_GameAcknowledge.h"
#include <cstdlib>
#include <iomanip>
#include <sstream>

DXP_GameAcknowledge::DXP_GameAcknowledge(const std::string& n, AcceptanceCode a)
:
        d_name_follower(n),
        d_acceptance_code(a)
{
}

DXP_GameAcknowledge::DXP_GameAcknowledge(const std::string& s)
:
        d_name_follower(s.substr(1, 32)),
        d_acceptance_code(static_cast<AcceptanceCode>(atoi(s.substr(33, 1).c_str())))
{
}

std::string DXP_GameAcknowledge::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw(32) << name_follower() << std::setfill(' ');
        sstr << std::setw( 1) << acceptance_code();

        return sstr.str();
}

const std::string& DXP_GameAcknowledge::name_follower(void) const
{
        return d_name_follower;
}

DXP_GameAcknowledge::AcceptanceCode DXP_GameAcknowledge::acceptance_code(void) const
{
        return d_acceptance_code;
}
