#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "GameRequest.h"
#include "Parser.h"

namespace DXP = damexchange;

const std::string DXP::layer2::GameRequest::HEADER = "R";

const char DXP::layer2::GameRequest::INITIAL = 'A';
const char DXP::layer2::GameRequest::SPECIAL = 'B';
const char DXP::layer2::GameRequest::SETUP[2] = {
        INITIAL,
        SPECIAL
};

const bool DXP::layer2::GameRequest::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<DXP::layer2::MessageInterface> DXP::layer2::GameRequest::create(const std::string& msg)
{
        return std::make_shared<GameRequest>(msg);
}

DXP::layer2::GameRequest::GameRequest(const std::string& msg)
:
        name_initiator_(msg.substr(2, 32)),
        color_follower_(*(msg.substr(34, 1)).begin()),
        minutes_(boost::lexical_cast<size_t>(msg.substr(35, 3).c_str())),
        moves_(boost::lexical_cast<size_t>(msg.substr(38, 3).c_str())),
        setup_position_(read_setup( *(msg.substr(41, 1)).begin() ))
{
        if (setup_position())
                special_position_ = msg.substr(42);
}

DXP::layer2::GameRequest::GameRequest(const std::string& n, char c, size_t min, size_t mov, bool s, const std::string& f)
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

const std::string& DXP::layer2::GameRequest::name_initiator(void) const
{
        return name_initiator_;
}

char DXP::layer2::GameRequest::color_follower(void) const
{
        return color_follower_;
}

size_t DXP::layer2::GameRequest::minutes(void) const
{
        return minutes_;
}

size_t DXP::layer2::GameRequest::moves(void) const
{
        return moves_;
}

bool DXP::layer2::GameRequest::setup_position(void) const
{
        return setup_position_;
}

const std::string& DXP::layer2::GameRequest::special_position(void) const
{
        return special_position_;
}

std::string DXP::layer2::GameRequest::header(void) const
{
        return HEADER;
}

std::string DXP::layer2::GameRequest::body(void) const
{
        std::stringstream sstr;
        sstr << std::setw( 2) << std::setfill('0') << PROTOCOL_VERSION;
        sstr << std::setw(32) << name_initiator() << std::setfill(' ');
        sstr << std::setw( 1) << color_follower();
        sstr << std::setw( 3) << std::setfill('0') << minutes();
        sstr << std::setw( 3) << std::setfill('0') << moves();
        sstr << std::setw( 1) << write_setup(setup_position());
        if (setup_position())
                sstr << std::setw(51) << special_position();
        return sstr.str();
}

bool DXP::layer2::GameRequest::read_setup(char c)
{
        switch(c) {                
        case INITIAL: 
                return false;
        case SPECIAL: 
                return true;
        default:
                assert(false);
                return false;
        }
}

char DXP::layer2::GameRequest::write_setup(bool b)
{
        return SETUP[b];
}
