#include "DXP_GameRequest.h"
#include "DXP_MessageFactory.h"
#include "../../IO/Token.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

const std::string DXP_GameRequest::HEADER = "R";

const bool DXP_GameRequest::REGISTERED = DXP_MessageFactory::register_creator(HEADER, create);

DXP_AbstractMessage* DXP_GameRequest::create(const DXP_StringMessage& s)
{
        assert(s.header() == HEADER);
        return new DXP_GameRequest(s.body());
}

DXP_GameRequest::DXP_GameRequest(const std::string& s)
:
        name_initiator_(s.substr(2, 32)),
        color_follower_(DXP_PositionToken::read_color( *(s.substr(34, 1)).begin() )),
        minutes_(atoi(s.substr(35, 3).c_str())),
        moves_(atoi(s.substr(38, 3).c_str())),
        setup_position_(DXP_PositionToken::read_setup( *(s.substr(41, 1)).begin() ))
{
        if (setup_position())
                special_position_ = s.substr(42);
}

DXP_GameRequest::DXP_GameRequest(const std::string& n, bool c, size_t min, size_t mov, bool s, const std::string& f)
:
        name_initiator_(n),
        color_follower_(c),
        minutes_(min),
        moves_(mov),
        setup_position_(s)
{
        if (setup_position())
                special_position_ = f;
}

DXP_StringMessage DXP_GameRequest::message(void) const
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

        return DXP_StringMessage(HEADER, sstr.str());
}

const std::string& DXP_GameRequest::name_initiator(void) const
{
        return name_initiator_;
}

bool DXP_GameRequest::color_follower(void) const
{
        return color_follower_;
}

size_t DXP_GameRequest::minutes(void) const
{
        return minutes_;
}

size_t DXP_GameRequest::moves(void) const
{
        return moves_;
}

bool DXP_GameRequest::setup_position(void) const
{
        return setup_position_;
}

const std::string& DXP_GameRequest::special_position(void) const
{
        return special_position_;
}
