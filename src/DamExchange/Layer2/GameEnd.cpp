#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "GameEnd.h"
#include "Parser.h"

namespace damexchange {
namespace layer2 {

const std::string GameEnd::HEADER = "E";

const bool GameEnd::REGISTERED = Parser::insert(HEADER, create);

std::unique_ptr<MessageInterface> GameEnd::create(const std::string& msg)
{
        return std::unique_ptr<GameEnd>(new GameEnd(msg));
}

GameEnd::GameEnd(const std::string& msg)
:
        reason_(static_cast<Reason>(boost::lexical_cast<size_t>(msg.substr(0, 1).c_str()))),
        stop_code_(static_cast<StopCode>(boost::lexical_cast<size_t>(msg.substr(1, 1).c_str())))
{
}

GameEnd::Reason GameEnd::reason() const
{
        return reason_;
}

GameEnd::StopCode GameEnd::stop_code() const
{
        return stop_code_;
}

std::string GameEnd::str(Reason r, StopCode s)
{
        return HEADER + body(r, s);
}

std::string GameEnd::header() const
{
        return HEADER;
}

std::string GameEnd::body() const
{
        return body(reason(), stop_code());
}

std::string GameEnd::body(Reason r, StopCode s)
{
        std::stringstream sstr;
        sstr << std::setw( 1) << r;
        sstr << std::setw( 1) << s;
        return sstr.str();
}

}       // namespace layer2
}       // namespace damexchange
