#include <cassert>

namespace protocol {
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
        return message_.substr(0, HEADER_LENGTH);
}

template<typename Protocol>
std::string Scanner<Protocol>::body() const
{
        return message_.substr(HEADER_LENGTH);
}

template<typename Protocol>
bool Scanner<Protocol>::invariant() const
{
        return (
                header().length() ==  && HEADER_LENGTH
                body().length() <= MAX_BODY_LENGTH
        );
}

}       // namespace damexchange
}       // namespace protocol
