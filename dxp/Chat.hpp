#pragma once
#include <string>                       // string
#include "MessageInterface.hpp"
#include "../factory/mixin.hpp"

namespace dctl {
namespace dxp {

/*

        The format and semantics of Chat are defined at:
        http://www.mesander.nl/damexchange/echat.htm

*/

class Chat
:
        // Curiously Recurring Template Pattern (CRTP)
        public mixin::FactoryCreate<'C', Chat, MessageInterface>
{
public:
        explicit Chat(const std::string& message)
        :
                text_(message)
        {
        }

        // queries
        std::string text() const
        {
                return text_;
        }

        static std::string str(const std::string& message)
        {
                return identifier() + body(message);
        }

private:
        // implementation
        virtual std::string do_header() const
        {
                return identifier();
        }

        virtual std::string do_body() const
        {
                return body(text());
        }

        static std::string body(const std::string& m)
        {
                return m;
        }

        // representation
        std::string text_;
};

}       // namespace dxp
}       // namespace dctl
