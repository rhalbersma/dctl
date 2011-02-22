#include "GameRequest.h"
#include "Parser.h"
#include "Token.h"
#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace DXP = DamExchange;

const std::string DXP::Layer2::GameRequest::HEADER = "R";

const bool DXP::Layer2::GameRequest::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<DXP::Layer2::AbstractMessage> DXP::Layer2::GameRequest::create(const std::string& msg)
{
        return std::make_shared<GameRequest>(msg);
}

DXP::Layer2::GameRequest::GameRequest(const std::string& msg)
:
        name_initiator_(msg.substr(2, 32)),
        color_follower_(PositionToken<DXP_tag>::read_color( *(msg.substr(34, 1)).begin() )),
        minutes_(boost::lexical_cast<size_t>(msg.substr(35, 3).c_str())),
        moves_(boost::lexical_cast<size_t>(msg.substr(38, 3).c_str())),
        setup_position_(PositionToken<DXP_tag>::read_setup( *(msg.substr(41, 1)).begin() ))
{
        if (setup_position())
                special_position_ = msg.substr(42);
}

DXP::Layer2::GameRequest::GameRequest(const std::string& n, bool c, size_t min, size_t mov, bool s, const std::string& f)
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

const std::string& DXP::Layer2::GameRequest::name_initiator(void) const
{
        return name_initiator_;
}

bool DXP::Layer2::GameRequest::color_follower(void) const
{
        return color_follower_;
}

size_t DXP::Layer2::GameRequest::minutes(void) const
{
        return minutes_;
}

size_t DXP::Layer2::GameRequest::moves(void) const
{
        return moves_;
}

bool DXP::Layer2::GameRequest::setup_position(void) const
{
        return setup_position_;
}

const std::string& DXP::Layer2::GameRequest::special_position(void) const
{
        return special_position_;
}

std::string DXP::Layer2::GameRequest::header(void) const
{
        return HEADER;
}

std::string DXP::Layer2::GameRequest::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw( 2) << std::setfill('0') << PROTOCOL_VERSION;
        sstr << std::setw(32) << name_initiator() << std::setfill(' ');
        sstr << std::setw( 1) << PositionToken<DXP_tag>::write_color(color_follower());
        sstr << std::setw( 3) << std::setfill('0') << minutes();
        sstr << std::setw( 3) << std::setfill('0') << moves();
        sstr << std::setw( 1) << PositionToken<DXP_tag>::write_setup(setup_position());
        if (setup_position())
                sstr << std::setw(51) << special_position();
        return sstr.str();
}
