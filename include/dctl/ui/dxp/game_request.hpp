#pragma once
#include <dctl/ui/dxp/message.hpp>      // Message
#include <dctl/utility/factory/creatable.hpp>   // make_creatable
#include <iomanip>                      // setfill, setw
#include <sstream>                      // stringstream
#include <string>                       // string

namespace dctl {
namespace dxp {

/*

        The format and semantics of GameRequest are defined at:
        http://www.mesander.nl/damexchange/egamereq.htm

*/

class GameRequest final
:
        // Curiously Recurring Template Pattern (CRTP)
        public factory::make_creatable<Message, GameRequest, 'R'>
{
public:
        static const auto protocol_version = 1;
        enum SetupCode { initial = 'A', special = 'B' };

        // constructors

        explicit GameRequest(std::string const& message)
        :
                name_initiator_{message.substr(2, 32)},
                Color_follower_ {*(std::begin(message.substr(34, 1)))},
                minutes_ {std::stoi(message.substr(35, 3).c_str())},
                moves_{std::stoi(message.substr(38, 3).c_str())},
                setup_code_{static_cast<SetupCode>(*(std::begin(message.substr(41, 1))))}
        {
                if (setup_code() == special)
                        position_ = message.substr(42);
        }

        // observers

        std::string const& name_initiator() const
        {
                return name_initiator_;
        }

        char Color_follower() const
        {
                return Color_follower_;
        }

        int minutes() const
        {
                return minutes_;
        }

        int moves() const
        {
                return moves_;
        }

        SetupCode setup_code() const
        {
                return setup_code_;
        }

        std::string const& position() const
        {
                return position_;
        }

        // output

        static std::string str(std::string const& n, char c, int min, int mov, SetupCode s, std::string const& p)
        {
                return identifier() + body(n, c, min, mov, s, p);
        }

private:
        // virtual implementation

        std::string do_header() const override
        {
                return identifier();
        }

        std::string do_body() const override
        {
                return body(name_initiator(), Color_follower(), minutes(), moves(), setup_code(), position());
        }

        static std::string body(std::string const& n, char c, int min, int mov, SetupCode s, std::string const& p)
        {
                std::stringstream sstr;
                sstr << std::setw( 2) << std::setfill('0') << protocol_version;
                sstr << std::setw(32) << n << std::setfill(' ');
                sstr << std::setw( 1) << c;
                sstr << std::setw( 3) << std::setfill('0') << min;
                sstr << std::setw( 3) << std::setfill('0') << mov;
                sstr << std::setw( 1) << static_cast<char>(s);
                if (s == special)
                        sstr << std::setw(51) << p;
                return sstr.str();
        }

        // representation

        std::string name_initiator_;
        char Color_follower_;
        int minutes_;
        int moves_;
        SetupCode setup_code_;
        std::string position_;
};

}       // namespace dxp
}       // namespace dctl
