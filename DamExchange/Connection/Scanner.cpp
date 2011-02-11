#include "Scanner.h"
#include <cassert>

namespace DXP = DamExchangeProtocol;
namespace Layer1 = DXP::Connection;

Layer1::Scanner::Scanner(const std::string& s)
:
        message_(s)
{
        assert(invariant());
}

std::string Layer1::Scanner::str(void) const
{
        return message_;
}

std::string Layer1::Scanner::header(void) const
{
        return str().substr(0, HEADER_LENGTH);
}

std::string Layer1::Scanner::body(void) const
{
        return str().substr(HEADER_LENGTH);
}

const char* Layer1::Scanner::c_str(void) const
{
        return str().c_str();
}

size_t Layer1::Scanner::length(void) const
{
        return str().length();
}

char Layer1::Scanner::terminator(void)
{
        return TERMINATOR;
}

bool Layer1::Scanner::invariant(void) const
{
        return (
                header().length() == HEADER_LENGTH && 
                body().length() <= MAX_BODY_LENGTH
        );
}
