#include <boost/assert.hpp>
#include "Chat.h"
#include "Parser.h"
#include "DXP.h"

namespace dctl {
namespace dxp {

const std::string Chat::HEADER = "C";

const bool Chat::REGISTERED = Parser<protocol>::register_message(HEADER, create);

std::unique_ptr<MessageInterface> Chat::create(const std::string& message)
{
        return std::unique_ptr<Chat>(new Chat(message));
}

Chat::Chat(const std::string& message)
: 
        message_(message)
{
}

std::string Chat::message() const
{
        return message_;
}

std::string Chat::str(const std::string& message)
{
        return HEADER + body(message);
}

std::string Chat::header() const
{
        return HEADER;
}

std::string Chat::body() const
{
        return body(message());
}

std::string Chat::body(const std::string& m)
{
        return m;
}

}       // namespace dxp
}       // namespace dctl
