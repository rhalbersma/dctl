#include "DXP_Chat.h"
#include "DXP_MessageFactory.h"
#include <cassert>

const std::string DXP_Chat::HEADER = "C";

const bool DXP_Chat::REGISTERED = DXP_MessageFactory::register_creator(HEADER, create);

DXP_AbstractMessage* DXP_Chat::create(const DXP_StringMessage& s)
{
        assert(s.header() == HEADER);
        return new DXP_Chat(s.body());
}

DXP_Chat::DXP_Chat(const std::string& s)
:
        message_(s)
{
}

DXP_StringMessage DXP_Chat::message(void) const
{
        return DXP_StringMessage(HEADER, message_);
}
