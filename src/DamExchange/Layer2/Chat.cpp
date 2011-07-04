#include <cassert>
#include "Chat.h"
#include "Parser.h"

namespace DXP = damexchange;

const std::string DXP::layer2::Chat::HEADER = "C";

const bool DXP::layer2::Chat::REGISTERED = Parser::insert(HEADER, create);

std::unique_ptr<DXP::layer2::MessageInterface> DXP::layer2::Chat::create(const std::string& msg)
{
        return std::unique_ptr<Chat>(new Chat(msg));
}

DXP::layer2::Chat::Chat(const std::string& msg)
: 
        message_(msg)
{
}

std::string DXP::layer2::Chat::message(void) const
{
        return message_;
}

std::string DXP::layer2::Chat::str(const std::string& msg)
{
        return HEADER + body(msg);
}

std::string DXP::layer2::Chat::header(void) const
{
        return HEADER;
}

std::string DXP::layer2::Chat::body(void) const
{
        return body(message());
}

std::string DXP::layer2::Chat::body(const std::string& m)
{
        return m;
}
