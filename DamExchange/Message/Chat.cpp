#include "Chat.h"
#include "Parser.h"
#include <cassert>

namespace DXP = DamExchangeProtocol;
namespace Layer2 = DXP::Message;

const std::string Layer2::Chat::HEADER = "C";

const bool Layer2::Chat::REGISTERED = Parser::insert(HEADER, create);

std::shared_ptr<Layer2::AbstractMessage> Layer2::Chat::create(const std::string& s)
{
        return std::make_shared<Chat>(s);
}

Layer2::Chat::Chat(const std::string& s)
:
        message_(s)
{
}

std::string Layer2::Chat::header(void) const
{
        return HEADER;
}

std::string Layer2::Chat::body(void) const
{
        return message_;
}
