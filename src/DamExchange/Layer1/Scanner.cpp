#include <cassert>
#include "Scanner.h"

namespace DXP = damexchange;

DXP::layer1::Scanner::Scanner(const std::string& msg)
:
        message_(msg)
{
        assert(invariant());
}

std::string DXP::layer1::Scanner::str(void) const
{
        return message_;
}

std::string DXP::layer1::Scanner::header(void) const
{
        return str().substr(0, HEADER_LENGTH);
}

std::string DXP::layer1::Scanner::body(void) const
{
        return str().substr(HEADER_LENGTH);
}

const char* DXP::layer1::Scanner::c_str(void) const
{
        return str().c_str();
}

size_t DXP::layer1::Scanner::length(void) const
{
        return str().length();
}

char DXP::layer1::Scanner::terminator(void)
{
        return TERMINATOR;
}

bool DXP::layer1::Scanner::invariant(void) const
{
        return (
                header().length() == HEADER_LENGTH && 
                body().length() <= MAX_BODY_LENGTH
        );
}
