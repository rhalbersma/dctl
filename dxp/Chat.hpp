#pragma once
#include <memory>                       // std::unique_ptr
#include <string>                       // std::string
#include "MessageInterface.hpp"

namespace dctl {
namespace dxp {

/*

        The Chat class is a <ConcreteProduct> in a <Factory Method>
        design pattern, with the Parser class as the <ConcreteCreator> 
        and the MessageInterface class as the <Product>.

        The Chat class registers itself with the factory.

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
                return HEADER_ + body(message);
        }

private:
        // factory creation
        static std::unique_ptr<MessageInterface> create(const std::string& message)
        {
                return std::unique_ptr<Chat>(new Chat(message));
        }

        // private constructor
        explicit Chat(const std::string& message)
        : 
                text_(message)
        {
        }

        // implementation
        virtual std::string header() const
        {
                return HEADER_;
        }

        virtual std::string body() const
        {
                return body(text());
        }

        static std::string body(const std::string& m)
        {
                return m;
        }

        static const std::string HEADER_;
        static const bool REGISTERED_;

        // representation
        std::string text_;
};

}       // namespace dxp
}       // namespace dctl
