#include "GameRequest.h"
#include "Parser.h"
#include "Token.h"
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace DXP = DamExchangeProtocol;
namespace Layer2 = DXP::Message;

const std::string Layer2::GameRequest::HEADER = "R";

const bool Layer2::GameRequest::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<Layer2::AbstractMessage> Layer2::GameRequest::create(const std::string& s)
{
        return std::make_shared<GameRequest>(s);
}

Layer2::GameRequest::GameRequest(const std::string& s)
:
        name_initiator_(s.substr(2, 32)),
        color_follower_(PositionToken<DXP_tag>::read_color( *(s.substr(34, 1)).begin() )),
        minutes_(atoi(s.substr(35, 3).c_str())),
        moves_(atoi(s.substr(38, 3).c_str())),
        setup_position_(PositionToken<DXP_tag>::read_setup( *(s.substr(41, 1)).begin() ))
{
        if (setup_position())
                special_position_ = s.substr(42);
}

Layer2::GameRequest::GameRequest(const std::string& n, bool c, size_t min, size_t mov, bool s, const std::string& f)
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

const std::string& Layer2::GameRequest::name_initiator(void) const
{
        return name_initiator_;
}

bool Layer2::GameRequest::color_follower(void) const
{
        return color_follower_;
}

size_t Layer2::GameRequest::minutes(void) const
{
        return minutes_;
}

size_t Layer2::GameRequest::moves(void) const
{
        return moves_;
}

bool Layer2::GameRequest::setup_position(void) const
{
        return setup_position_;
}

const std::string& Layer2::GameRequest::special_position(void) const
{
        return special_position_;
}

std::string Layer2::GameRequest::header(void) const
{
        return HEADER;
}

std::string Layer2::GameRequest::body(void) const
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
