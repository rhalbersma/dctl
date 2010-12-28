#include "DXP_GameRequest.h"
#include "../../IO/Token.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

DXP_GameRequest::DXP_GameRequest(const std::string& n, bool c, size_t min, size_t mov, bool s, const std::string& f)
:
        d_name_initiator(n),
        d_color_follower(c),
        d_minutes(min),
        d_moves(mov),
        d_setup_position(s)
{
        if (setup_position())
                d_special_position = f;
}

DXP_GameRequest::DXP_GameRequest(const std::string& s)
:
        d_name_initiator(s.substr(3, 32)),
        d_color_follower(DXP_PositionToken::read_color( *(s.substr(35, 1)).begin() )),
        d_minutes(atoi(s.substr(36, 3).c_str())),
        d_moves(atoi(s.substr(39, 3).c_str())),
        d_setup_position(DXP_PositionToken::read_setup( *(s.substr(42, 1)).begin() ))
{
        if (setup_position())
                d_special_position = s.substr(43);

        assert(invariant(*s.begin()));
}

char DXP_GameRequest::header(void) const
{
        return HEADER;
}

std::string DXP_GameRequest::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 1) << HEADER;
        sstr << std::setw( 2) << std::setfill('0') << PROTOCOL_VERSION;
        sstr << std::setw(32) << name_initiator() << std::setfill(' ');
        sstr << std::setw( 1) << DXP_PositionToken::write_color(color_follower());
        sstr << std::setw( 3) << std::setfill('0') << minutes();
        sstr << std::setw( 3) << std::setfill('0') << moves();
        sstr << std::setw( 1) << DXP_PositionToken::write_setup(setup_position());
        if (setup_position())
                sstr << std::setw(51) << special_position();

        return sstr.str();
}

const std::string& DXP_GameRequest::name_initiator(void) const
{
        return d_name_initiator;
}

bool DXP_GameRequest::color_follower(void) const
{
        return d_color_follower;
}

size_t DXP_GameRequest::minutes(void) const
{
        return d_minutes;
}

size_t DXP_GameRequest::moves(void) const
{
        return d_moves;
}

bool DXP_GameRequest::setup_position(void) const
{
        return d_setup_position;
}

const std::string& DXP_GameRequest::special_position(void) const
{
        return d_special_position;
}
