#include <cassert>
#include "Chat.h"
#include "Parser.h"

namespace DXP = damexchange;

const std::string DXP::layer2::Chat::HEADER = "C";

const bool DXP::layer2::Chat::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<DXP::layer2::MessageInterface> DXP::layer2::Chat::create(const std::string& msg)
{
        return std::make_shared<Chat>(msg);
}

DXP::layer2::Chat::Chat(const std::string& msg)
:
        message_(msg)
{
}

std::string DXP::layer2::Chat::header(void) const
{
        return HEADER;
}

std::string DXP::layer2::Chat::body(void) const
{
        return message_;
}
