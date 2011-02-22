#include "Chat.h"
#include "Parser.h"
#include <cassert>

namespace DXP = DamExchange;

const std::string DXP::Layer2::Chat::HEADER = "C";

const bool DXP::Layer2::Chat::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<DXP::Layer2::AbstractMessage> DXP::Layer2::Chat::create(const std::string& msg)
{
        return std::make_shared<Chat>(msg);
}

DXP::Layer2::Chat::Chat(const std::string& msg)
:
        message_(msg)
{
}

std::string DXP::Layer2::Chat::header(void) const
{
        return HEADER;
}

std::string DXP::Layer2::Chat::body(void) const
{
        return message_;
}
