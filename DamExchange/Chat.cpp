#include "Chat.h"
#include "MessageFactory.h"
#include <cassert>

namespace DXP = DamExchangeProtocol;

const std::string DXP::Chat::HEADER = "C";

const bool DXP::Chat::REGISTERED = MessageFactory::register_creator(HEADER, create);

std::shared_ptr<DXP::AbstractMessage> DXP::Chat::create(const StringMessage& s)
{
        assert(pre_condition(s));
        return std::make_shared<Chat>(s.body());
}

DXP::Chat::Chat(const std::string& s)
:
        message_(s)
{
}

std::string DXP::Chat::header(void) const
{
        return HEADER;
}

std::string DXP::Chat::body(void) const
{
        return message_;
}

bool DXP::Chat::pre_condition(const StringMessage& s)
{
        return REGISTERED && HEADER == s.header();
}
