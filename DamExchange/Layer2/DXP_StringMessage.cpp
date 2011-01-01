#include "DXP_StringMessage.h"
#include <cassert>

DXP_StringMessage::DXP_StringMessage(const std::string& s)
:
        content_(s)
{
        assert(invariant());
}

DXP_StringMessage::DXP_StringMessage(const std::string& header_, const std::string& body_)
:
        content_(header_ + body_)
{
        assert(invariant(header_, body_));
}

const char* DXP_StringMessage::c_str(void) const
{
        return str().c_str();
}

std::string DXP_StringMessage::str(void) const
{
        return content_;
}

std::string DXP_StringMessage::header(void) const
{
        return str().substr(0, HEADER_LENGTH);
}

std::string DXP_StringMessage::body(void) const
{
        return str().substr(HEADER_LENGTH);
}

bool DXP_StringMessage::invariant(void) const
{
        return (
                header().length() == HEADER_LENGTH && 
                body().length() <= MAX_BODY_LENGTH
        );
}

bool DXP_StringMessage::invariant(const std::string& header_, const std::string& body_) const
{
        return (
                header_.length() == HEADER_LENGTH && 
                body_.length() <= MAX_BODY_LENGTH
        );
}
