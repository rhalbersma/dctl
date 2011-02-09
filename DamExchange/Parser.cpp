#include "Parser.h"
#include <cassert>

namespace DXP = DamExchangeProtocol;

DXP::Parser::Parser(const std::string& s)
:
        content_(s)
{
        assert(invariant());
}

DXP::Parser::Parser(const std::string& h, const std::string& b)
:
        content_(h + b)
{
        assert(invariant(h, b));
}

std::string DXP::Parser::str(void) const
{
        return content_;
}

std::string DXP::Parser::header(void) const
{
        return str().substr(0, HEADER_LENGTH);
}

std::string DXP::Parser::body(void) const
{
        return str().substr(HEADER_LENGTH);
}

const char* DXP::Parser::c_str(void) const
{
        return str().c_str();
}

size_t DXP::Parser::length(void) const
{
        return str().length();
}

char DXP::Parser::terminator(void)
{
        return TERMINATOR;
}

bool DXP::Parser::invariant(void) const
{
        return (
                header().length() == HEADER_LENGTH && 
                body().length() <= MAX_BODY_LENGTH
        );
}

bool DXP::Parser::invariant(const std::string& header_, const std::string& body_) const
{
        return (
                header_.length() == HEADER_LENGTH && 
                body_.length() <= MAX_BODY_LENGTH
        );
}
