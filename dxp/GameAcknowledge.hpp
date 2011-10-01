#pragma once
#include <iomanip>                      // std::setfill, std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"
#include "Mixin.hpp"

namespace dctl {
namespace dxp {

/*

        The GameAcknowledge class is a <ConcreteProduct> in a <Factory Method>
        design pattern, with the Parser class as the <ConcreteCreator> and the 
        MessageInterface class as the <Product>.

        The GameAcknowledge class MUST be registered with a factory.

        The format and semantics of GameAcknowledge are defined at:
        http://www.mesander.nl/damexchange/egameacc.htm

*/
        
class GameAcknowledge
: 
        public MessageInterface
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

        static std::string generate(const std::string& n, AcceptanceCode a)
        {
                return header() + body(n, a);
        }

        // factory creation (NOTE: makes constructor private)
        MIXIN_FACTORY_CREATION(GameAcknowledge)

        explicit GameAcknowledge(const std::string& message)
        :
                name_follower_(message.substr(0, 32)),
                acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<int>(message.substr(32, 1).c_str())))
        {
        }

private:
        // implementation
        MIXIN_MESSAGE_HEADER('A')

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

        // representation
        std::string name_follower_;
        AcceptanceCode acceptance_code_;
};

}       // namespace dxp
}       // namespace dctl
