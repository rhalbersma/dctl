#include <cassert>
#include "Protocol.h"

namespace damexchange {

template<typename Protocol>
Scanner<Protocol>::Scanner(const std::string& message)
:
        message_(message)
{
        assert(invariant());
}

template<typename Protocol>
std::string Scanner<Protocol>::header() const
{
        return message_.substr(0, header_length<Protocol>::value);
}

template<typename Protocol>
std::string Scanner<Protocol>::body() const
{
        return message_.substr(header_length<Protocol>::value);
}

template<typename Protocol>
bool Scanner<Protocol>::invariant() const
{
        return (
                header().length() == header_length<Protocol>::value && 
                body().length() <= max_body_length<Protocol>::value
        );
}

}       // namespace damexchange
