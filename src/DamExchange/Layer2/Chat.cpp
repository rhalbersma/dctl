#include <cassert>
#include "Chat.h"
#include "Parser.h"

namespace damexchange {
namespace layer2 {

const std::string Chat::HEADER = "C";

const bool Chat::REGISTERED = Parser::insert(HEADER, create);

std::unique_ptr<MessageInterface> Chat::create(const std::string& msg)
{
        return std::unique_ptr<Chat>(new Chat(msg));
}

Chat::Chat(const std::string& msg)
: 
        message_(msg)
{
}

std::string Chat::message() const
{
        return message_;
}

std::string Chat::str(const std::string& msg)
{
        return HEADER + body(msg);
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

}       // namespace layer2
}       // namespace damexchange
