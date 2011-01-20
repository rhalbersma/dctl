#include "StringMessage.h"
#include <cassert>

namespace DXP  = DamExchangeProtocol;

DXP::StringMessage::StringMessage(const std::string& s)
:
        content_(s)
{
        assert(invariant());
}

DXP::StringMessage::StringMessage(const std::string& h, const std::string& b)
:
        content_(h + b)
{
        assert(invariant(h, b));
}

const char* DXP::StringMessage::c_str(void) const
{
        return str().c_str();
}

std::string DXP::StringMessage::str(void) const
{
        return content_;
}

std::string DXP::StringMessage::header(void) const
{
        return str().substr(0, HEADER_LENGTH);
}

std::string DXP::StringMessage::body(void) const
{
        return str().substr(HEADER_LENGTH);
}

bool DXP::StringMessage::invariant(void) const
{
        return (
                header().length() == HEADER_LENGTH && 
                body().length() <= MAX_BODY_LENGTH
        );
}

bool DXP::StringMessage::invariant(const std::string& header_, const std::string& body_) const
{
        return (
                header_.length() == HEADER_LENGTH && 
                body_.length() <= MAX_BODY_LENGTH
        );
}
