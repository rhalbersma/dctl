#include <cassert>
#include "Chat.h"
#include "Parser.h"
#include "Version.h"

namespace dctl {
namespace protocol {
namespace dxp {

const std::string Chat::HEADER = "C";

const bool Chat::REGISTERED = Parser<version>::insert(HEADER, create);

std::unique_ptr<MessageInterface> Chat::create(const std::string& message)
{
        return std::unique_ptr<Chat>(new Chat(message));
}

Chat::Chat(const std::string& message)
: 
        message_(message)
{
}

std::string Chat::message() const
{
        return message_;
}

std::string Chat::str(const std::string& message)
{
        return HEADER + body(message);
}

std::string Chat::header() const
{
        return HEADER;
}

std::string Chat::body() const
{
        return body(message());
}

std::string Chat::body(const std::string& m)
{
        return m;
}

}       // namespace dxp
}       // namespace protocol
}       // namespace dctl
