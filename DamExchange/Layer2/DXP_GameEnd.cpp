#include "DXP_GameEnd.h"
#include "DXP_MessageFactory.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

const std::string DXP_GameEnd::HEADER = "E";

const bool DXP_GameEnd::REGISTERED = DXP_MessageFactory::register_creator(HEADER, create);

DXP_AbstractMessage* DXP_GameEnd::create(const DXP_StringMessage& s)
{
        assert(s.header() == HEADER);
        return new DXP_GameEnd(s.body());
}

DXP_GameEnd::DXP_GameEnd(const std::string& s)
:
        reason_(static_cast<Reason>(atoi(s.substr(0, 1).c_str()))),
        stop_code_(static_cast<StopCode>(atoi(s.substr(1, 1).c_str())))
{
}

DXP_GameEnd::DXP_GameEnd(Reason r, StopCode s)
:
        reason_(r),
        stop_code_(s)
{
}

DXP_StringMessage DXP_GameEnd::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << reason();
        sstr << std::setw( 1) << stop_code();

        return DXP_StringMessage(HEADER, sstr.str());
}

DXP_GameEnd::Reason DXP_GameEnd::reason(void) const
{
        return reason_;
}

DXP_GameEnd::StopCode DXP_GameEnd::stop_code(void) const
{
        return stop_code_;
}
