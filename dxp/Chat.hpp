#pragma once
#include <memory>                       // unique_ptr
#include <string>                       // string
#include "MessageInterface.hpp"
#include "../factory/mixin.hpp"
#include "../utility/make_unique.hpp"   // make_unique

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
private:
        friend std::unique_ptr<Chat> dctl::make_unique<Chat, std::string>(const std::string&);

        // structors

        explicit Chat(const std::string& message)
        :
                text_(message)
        {
        }

public:
        // queries

        std::string text() const
        {
                return text_;
        }

        // output

        static std::string str(const std::string& message)
        {
                return identifier() + body(message);
        }

private:
        // virtual implementation

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
