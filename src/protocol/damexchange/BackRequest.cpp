#include <cassert>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "BackRequest.h"
#include "Parser.h"
#include "Protocol.h"

namespace protocol {
namespace damexchange {

const std::string BackRequest::HEADER = "B";

const bool BackRequest::REGISTERED = Parser<version>::insert(HEADER, create);

std::unique_ptr<MessageInterface> BackRequest::create(const std::string& message)
{
        return std::unique_ptr<BackRequest>(new BackRequest(message));
}

BackRequest::BackRequest(const std::string& message)
:
        move_number_(boost::lexical_cast<size_t>(message.substr(0, 3).c_str())),
        side_to_move_(*(message.substr(3, 1)).begin())
{
}

size_t BackRequest::move_number() const
{
        return move_number_;
}

char BackRequest::side_to_move() const
{
        return side_to_move_;
}

std::string BackRequest::str(size_t m, char c)
{
        return HEADER + body(m, c);
}

std::string BackRequest::header() const
{
        return HEADER;
}

std::string BackRequest::body() const
{
        return body(move_number(), side_to_move());
}

std::string BackRequest::body(size_t m, char c)
{
        std::stringstream sstr;
        sstr << std::setw( 3) << std::setfill('0') << m;
        sstr << std::setw( 1) << c;
        return sstr.str();
}

}       // namespace damexchange
}       // namespace protocol
