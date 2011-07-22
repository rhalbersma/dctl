#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "GameAcknowledge.h"
#include "Parser.h"
#include "Protocol.h"

namespace dctl {
namespace protocol {
namespace damexchange {

const std::string GameAcknowledge::HEADER = "A";

const bool GameAcknowledge::REGISTERED = Parser<version>::insert(HEADER, create);

std::unique_ptr<MessageInterface> GameAcknowledge::create(const std::string& message)
{
        return std::unique_ptr<GameAcknowledge>(new GameAcknowledge(message));
}

GameAcknowledge::GameAcknowledge(const std::string& message)
:
        name_follower_(message.substr(0, 32)),
        acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<size_t>(message.substr(32, 1).c_str())))
{
}

const std::string& GameAcknowledge::name_follower() const
{
        return name_follower_;
}

GameAcknowledge::AcceptanceCode GameAcknowledge::acceptance_code() const
{
        return acceptance_code_;
}

std::string GameAcknowledge::str(const std::string& n, AcceptanceCode a)
{
        return HEADER + body(n, a);
}

std::string GameAcknowledge::header() const
{
        return HEADER;
}

std::string GameAcknowledge::body() const
{
        return body(name_follower(), acceptance_code());
}
        
std::string GameAcknowledge::body(const std::string& n, AcceptanceCode a)
{
        std::stringstream sstr;
        sstr << std::setw(32) << n << std::setfill(' ');
        sstr << std::setw( 1) << a;
        return sstr.str();
}

}       // namespace damexchange
}       // namespace protocol
}       // namespace dctl
