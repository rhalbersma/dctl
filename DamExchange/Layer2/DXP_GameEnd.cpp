#include "DXP_GameEnd.h"
#include "DXP_MessageFactory.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

const bool DXP_GameEnd::REGISTERED = DXP_MessageFactory::register_creator(HEADER, create);

DXP_AbstractMessage* DXP_GameEnd::create(const DXP_String& s)
{
        assert(s.header() == HEADER);
        return new DXP_GameEnd(s.content());
}

DXP_GameEnd::DXP_GameEnd(const std::string& s)
:
        d_reason(static_cast<Reason>(atoi(s.substr(0, 1).c_str()))),
        d_stop_code(static_cast<StopCode>(atoi(s.substr(1, 1).c_str())))
{
}

DXP_GameEnd::DXP_GameEnd(Reason r, StopCode s)
:
        d_reason(r),
        d_stop_code(s)
{
}

DXP_String DXP_GameEnd::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << reason();
        sstr << std::setw( 1) << stop_code();

        return DXP_String(HEADER, sstr.str());
}

DXP_GameEnd::Reason DXP_GameEnd::reason(void) const
{
        return d_reason;
}

DXP_GameEnd::StopCode DXP_GameEnd::stop_code(void) const
{
        return d_stop_code;
}
