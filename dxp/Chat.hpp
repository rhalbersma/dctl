#pragma once
#include <memory>                       // std::unique_ptr
#include <string>                       // std::string
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "MessageInterface.hpp"
#include "Mixin.hpp"

namespace dctl {
namespace dxp {

/*

        The Chat class is a <ConcreteProduct> in a <Factory Method>
        Design Pattern, with the Parser class as the <ConcreteCreator> 
        and the MessageInterface class as the <Product>.

        The Chat class MUST be registered with a factory.

        The format and semantics of Chat are defined at:
        http://www.mesander.nl/damexchange/echat.htm

*/
        
class Chat
: 
        public MessageInterface
{
public:
        // views
        std::string text() const
        {
                return text_;
        }

        static std::string generate(const std::string& message)
        {
                return header() + body(message);
        }

        // factory registration (NOTE: makes constructor private)
        MIXIN_HEADER_FACTORY_CREATION('C', Chat)

        explicit Chat(const std::string& message)
        : 
                text_(message)
        {
        }

private:
        // implementation
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
