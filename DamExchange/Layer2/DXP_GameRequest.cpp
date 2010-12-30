#include "DXP_GameRequest.h"
#include "DXP_MessageFactory.h"
#include "../../IO/Token.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

const bool DXP_GameRequest::REGISTERED = DXP_MessageFactory::register_creator(HEADER, create);

DXP_AbstractMessage* DXP_GameRequest::create(const DXP_String& s)
{
        assert(s.header() == HEADER);
        return new DXP_GameRequest(s.content());
}

DXP_GameRequest::DXP_GameRequest(const std::string& s)
:
        d_name_initiator(s.substr(2, 32)),
        d_color_follower(DXP_PositionToken::read_color( *(s.substr(34, 1)).begin() )),
        d_minutes(atoi(s.substr(35, 3).c_str())),
        d_moves(atoi(s.substr(38, 3).c_str())),
        d_setup_position(DXP_PositionToken::read_setup( *(s.substr(41, 1)).begin() ))
{
        if (setup_position())
                d_special_position = s.substr(42);
}

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

DXP_String DXP_GameRequest::message(void) const
{
        std::stringstream sstr;

        sstr << std::setw( 2) << std::setfill('0') << PROTOCOL_VERSION;
        sstr << std::setw(32) << name_initiator() << std::setfill(' ');
        sstr << std::setw( 1) << DXP_PositionToken::write_color(color_follower());
        sstr << std::setw( 3) << std::setfill('0') << minutes();
        sstr << std::setw( 3) << std::setfill('0') << moves();
        sstr << std::setw( 1) << DXP_PositionToken::write_setup(setup_position());
        if (setup_position())
                sstr << std::setw(51) << special_position();

        return DXP_String(HEADER, sstr.str());
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
