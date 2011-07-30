#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "GameRequest.h"
#include "Parser.h"
#include "DXP.h"

namespace dctl {
namespace dxp {

const std::string GameRequest::HEADER = "R";

const char GameRequest::INITIAL = 'A';
const char GameRequest::SPECIAL = 'B';
const char GameRequest::SETUP[2] = {
        INITIAL,
        SPECIAL
};

const bool GameRequest::REGISTERED = Parser<protocol>::insert(HEADER, create);

std::unique_ptr<MessageInterface> GameRequest::create(const std::string& message)
{
        return std::unique_ptr<GameRequest>(new GameRequest(message));
}

GameRequest::GameRequest(const std::string& message)
:
        name_initiator_(message.substr(2, 32)),
        color_follower_(*(message.substr(34, 1)).begin()),
        minutes_(boost::lexical_cast<size_t>(message.substr(35, 3).c_str())),
        moves_(boost::lexical_cast<size_t>(message.substr(38, 3).c_str())),
        setup_(read_setup( *(message.substr(41, 1)).begin() ))
{
        if (setup())
                position_ = message.substr(42);
}

const std::string& GameRequest::name_initiator() const
{
        return name_initiator_;
}

char GameRequest::color_follower() const
{
        return color_follower_;
}

size_t GameRequest::minutes() const
{
        return minutes_;
}

size_t GameRequest::moves() const
{
        return moves_;
}

bool GameRequest::setup() const
{
        return setup_;
}

const std::string& GameRequest::position() const
{
        return position_;
}

std::string GameRequest::str(const std::string& n, char c, size_t min, size_t mov, bool s, const std::string& p)
{
        return HEADER + body(n, c, min, mov, s, p);
}

std::string GameRequest::header() const
{
        return HEADER;
}

std::string GameRequest::body() const
{
        return body(name_initiator(), color_follower(), minutes(), moves(), setup(), position());
}

std::string GameRequest::body(const std::string& n, char c, size_t min, size_t mov, bool s, const std::string& p)
{
        std::stringstream sstr;
        sstr << std::setw( 2) << std::setfill('0') << PROTOCOL_VERSION;
        sstr << std::setw(32) << n << std::setfill(' ');
        sstr << std::setw( 1) << c;
        sstr << std::setw( 3) << std::setfill('0') << min;
        sstr << std::setw( 3) << std::setfill('0') << mov;
        sstr << std::setw( 1) << write_setup(s);
        if (s)
                sstr << std::setw(51) << p;
        return sstr.str();
}

bool GameRequest::read_setup(char c)
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

char GameRequest::write_setup(bool b)
{
        return SETUP[b];
}

}       // namespace dxp
}       // namespace dctl
