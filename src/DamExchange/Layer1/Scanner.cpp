#include <cassert>
#include "Scanner.h"

namespace damexchange {
namespace layer1 {

Scanner::Scanner(const std::string& msg)
:
        message_(msg)
{
        assert(invariant());
}

std::string Scanner::str() const
{
        return message_;
}

std::string Scanner::header() const
{
        return str().substr(0, HEADER_LENGTH);
}

std::string Scanner::body() const
{
        return str().substr(HEADER_LENGTH);
}

const char* Scanner::c_str() const
{
        return str().c_str();
}

size_t Scanner::length() const
{
        return str().length();
}

char Scanner::terminator()
{
        return TERMINATOR;
}

bool Scanner::invariant() const
{
        return (
                header().length() == HEADER_LENGTH && 
                body().length() <= MAX_BODY_LENGTH
        );
}

}       // namespace layer1
}       // namespace damexchange
