#pragma once
#include <iomanip>                      // setfill, setw
#include <sstream>                      // stringstream
#include <string>                       // stoi, string
#include <dctl/dxp/i_message.hpp>       // IMessage
#include <dctl/factory/mixin.hpp>       // FactoryCreate

namespace dctl {
namespace dxp {

/*

        The format and semantics of GameAcknowledge are defined at:
        http://www.mesander.nl/damexchange/egameacc.htm

*/

class GameAcknowledge
:
        // Curiously Recurring Template Pattern (CRTP)
        public mixin::FactoryCreate<'A', GameAcknowledge, IMessage>
{
public:
        // typedefs

        enum AcceptanceCode { accept = 0, decline_version = 1, decline_game = 2, decline_always = 3 };

        // structors

        explicit GameAcknowledge(std::string const& message)
        :
                name_follower_ { message.substr(0, 32) },
                acceptance_code_ { static_cast<AcceptanceCode>(std::stoi(message.substr(32, 1).c_str())) }
        {}

        // queries

        std::string const& name_follower() const
        {
                return name_follower_;
        }

        AcceptanceCode acceptance_code() const
        {
                return acceptance_code_;
        }

        static std::string generate(std::string const& n, AcceptanceCode a)
        {
                return identifier() + body(n, a);
        }

private:
        // virtual implementation

        virtual std::string do_header() const
        {
                return identifier();
        }

        virtual std::string do_body() const
        {
                return body(name_follower(), acceptance_code());
        }

        static std::string body(std::string const& n, AcceptanceCode a)
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
