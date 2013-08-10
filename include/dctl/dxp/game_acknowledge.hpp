#pragma once
#include <iomanip>                      // setfill, setw
#include <sstream>                      // stringstream
#include <string>                       // stoi, string
#include <dctl/dxp/message.hpp>         // Message
#include <dctl/factory/creatable.hpp>   // make_creatable

namespace dctl {
namespace dxp {

/*

        The format and semantics of GameAcknowledge are defined at:
        http://www.mesander.nl/damexchange/egameacc.htm

*/

class GameAcknowledge final
:
        // Curiously Recurring Template Pattern (CRTP)
        public factory::make_creatable<Message, GameAcknowledge, 'A'>
{
public:
        enum AcceptanceCode: int { accept = 0, decline_version = 1, decline_game = 2, decline_always = 3 };

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

        std::string do_header() const override
        {
                return identifier();
        }

        std::string do_body() const override
        {
                return body(name_follower(), acceptance_code());
        }

        static std::string body(std::string const& n, AcceptanceCode a)
        {
                std::stringstream sstr;
                sstr << std::setw(32) << n << std::setfill(' ');
                sstr << std::setw( 1) << static_cast<int>(a);
                return sstr.str();
        }

        // representation

        std::string name_follower_;
        AcceptanceCode acceptance_code_;
};

}       // namespace dxp
}       // namespace dctl
