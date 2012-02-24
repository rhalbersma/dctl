#pragma once
#include <iomanip>                      // setfill, setw
#include <sstream>                      // stringstream
#include <string>                       // string
#include <boost/lexical_cast.hpp>       // lexical_cast
#include "MessageInterface.hpp"
#include "../factory/mixin.hpp"

namespace dctl {
namespace dxp {

/*

        The format and semantics of GameAcknowledge are defined at:
        http://www.mesander.nl/damexchange/egameacc.htm

*/
        
class GameAcknowledge
: 
        public mixin::IdentifierCreate<'A', GameAcknowledge, MessageInterface>
{
public:
        // typedefs
        enum AcceptanceCode { accept = 0, decline_version = 1, decline_game = 2, decline_always = 3 };

        explicit GameAcknowledge(const std::string& message)
        :
                name_follower_(message.substr(0, 32)),
                acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<int>(message.substr(32, 1).c_str())))
        {
        }

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
                return identifier() + body(n, a);
        }

private:
        // implementation
        virtual std::string do_header() const
        {
                return identifier();
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

        // representation
        std::string name_follower_;
        AcceptanceCode acceptance_code_;
};

}       // namespace dxp
}       // namespace dctl
