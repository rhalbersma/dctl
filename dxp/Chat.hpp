#pragma once
#include <memory>                       // std::unique_ptr
#include <string>                       // std::string
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "MessageInterface.hpp"
#include "Parser.hpp"
#include "Protocol.hpp"

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
        
template
<
        typename Protocol = protocol_v1,
        template<typename> class Interface = MessageInterface, 
        typename Factory = Parser
>
class Chat_
: 
        public Interface<Protocol>
{
public:
        // views
        std::string text() const
        {
                return text_;
        }

        static std::string header()
        {
                return std::string(1, HEADER_);
        }

        static std::string generate(const std::string& message)
        {
                return header() + body(message);
        }

private:
        // factory creation
        typedef Interface<Protocol> InterfaceVersion;
        static std::unique_ptr<InterfaceVersion> create(const std::string& message)
        {
                return std::unique_ptr<Chat_>(new Chat_(message));
        }

        // private constructor
        explicit Chat_(const std::string& message)
        : 
                text_(message)
        {
        }

        // implementation
        virtual std::string do_header() const
        {
                return header();
        }

        virtual std::string do_body() const
        {
                return body(text());
        }

        static std::string body(const std::string& m)
        {
                return m;
        }

        BOOST_STATIC_CONSTANT(auto, HEADER_ = 'C');
        static bool registered_;

        // representation
        std::string text_;
};

template<typename Protocol, template<typename> class Interface, typename Factory>
bool Chat_<Protocol, Interface, Factory>::registered_ = 
        Factory::register_message(header(), create)
;

template class Chat_<>;
typedef Chat_<> Chat;

}       // namespace dxp
}       // namespace dctl
