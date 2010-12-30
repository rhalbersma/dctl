#include "DXP_Chat.h"
#include "DXP_MessageFactory.h"
#include <cassert>

const bool DXP_Chat::REGISTERED = DXP_MessageFactory::register_creator(HEADER, create);

DXP_AbstractMessage* DXP_Chat::create(const DXP_String& s)
{
        assert(s.header() == HEADER);
        return new DXP_Chat(s.content());
}

DXP_Chat::DXP_Chat(const std::string& s)
:
        d_message(s)
{
}

DXP_String DXP_Chat::message(void) const
{
        return DXP_String(HEADER, d_message);
}
