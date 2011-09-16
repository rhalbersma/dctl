#pragma once
#include <iomanip>                      // std::setfill, std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"
#include "Parser.hpp"
#include "Version.hpp"

namespace dctl {
namespace dxp {

/*

        The GameAcknowledge class is a <ConcreteProduct> in a <Factory Method>
        design pattern, with the Parser class as the <ConcreteCreator> and the 
        MessageInterface class as the <Product>.

        The GameAcknowledge class registers itself with the factory.

        The format and semantics of GameAcknowledge are defined at:
        http://www.mesander.nl/damexchange/egameacc.htm

*/
        
template
<
        typename Protocol = protocol,
        template<typename> class Interface = MessageInterface, 
        typename Factory = Parser
>
class GameAcknowledge_
: 
        public Interface<Protocol>
{
public:
        // typedefs
        enum AcceptanceCode { accept = 0, decline_version = 1, decline_game = 2, decline_always = 3 };

        // views
        const std::string& name_follower() const
        {
                return name_follower_;
        }

        AcceptanceCode acceptance_code() const
        {
                return acceptance_code_;
        }

        static std::string header()
        {
                return std::string(1, HEADER_);
        }

        static std::string generate(const std::string& n, AcceptanceCode a)
        {
                return header() + body(n, a);
        }

private:
        // factory creation
        typedef Interface<Protocol> InterfaceVersion;
        static std::unique_ptr<InterfaceVersion> create(const std::string& message)
        {
                return std::unique_ptr<GameAcknowledge_>(new GameAcknowledge_(message));
        }

        // private constructor
        explicit GameAcknowledge_(const std::string& message)
        :
                name_follower_(message.substr(0, 32)),
                acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<int>(message.substr(32, 1).c_str())))
        {
        }

        // implementation
        virtual std::string do_header() const
        {
                return header();
        }

        virtual std::string do_body() const
        {
                return body(name_follower(), acceptance_code());
        }

        static std::string body(const std::string& n, AcceptanceCode a)
        {
                std::stringstream sstr;
                sstr << std::setw(32) << n << std::setfill(' ');
                sstr << std::setw( 1) << a;
                return sstr.str();
        }

        BOOST_STATIC_CONSTANT(auto, HEADER_ = 'A');
        static bool registered_;

        // representation
        std::string name_follower_;
        AcceptanceCode acceptance_code_;
};

// factory registration
template<typename Protocol, template<typename> class Interface, typename Factory>
bool GameAcknowledge_<Protocol, Interface, Factory>::registered_ = 
        Factory::register_message(header(), create)
;

// explicit instantation
template class GameAcknowledge_<>;
typedef GameAcknowledge_<> GameAcknowledge;

}       // namespace dxp
}       // namespace dctl
