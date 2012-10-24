#pragma once
#include <iomanip>                      // setfill, setw
#include <sstream>                      // stringstream
#include <string>                       // string
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/lexical_cast.hpp>       // lexical_cast
#include <dctl/dxp/IMessage.hpp>             // IMessage
#include <dctl/factory/mixin.hpp>            // FactoryCreate

namespace dctl {
namespace dxp {

/*

        The format and semantics of GameRequest are defined at:
        http://www.mesander.nl/damexchange/egamereq.htm

*/

class GameRequest
:
        // Curiously Recurring Template Pattern (CRTP)
        public mixin::FactoryCreate<'R', GameRequest, IMessage>
{
public:
        // constants and typedefs

        BOOST_STATIC_CONSTANT(auto, protocol_version = 1);
        enum SetupCode { initial = 'A', special = 'B' };

        // structors

        explicit GameRequest(std::string const& message)
        :
                name_initiator_(message.substr(2, 32)),
                color_follower_(*(std::begin(message.substr(34, 1)))),
                minutes_(boost::lexical_cast<int>(message.substr(35, 3).c_str())),
                moves_(boost::lexical_cast<int>(message.substr(38, 3).c_str())),
                setup_code_(static_cast<SetupCode>(boost::lexical_cast<char>(*(std::begin(message.substr(41, 1))))))
        {
                if (setup_code() == special)
                        position_ = message.substr(42);
        }

        // queries

        std::string const& name_initiator() const
        {
                return name_initiator_;
        }

        char color_follower() const
        {
                return color_follower_;
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

        virtual std::string do_header() const
        {
                return identifier();
        }

        virtual std::string do_body() const
        {
                return body(name_initiator(), color_follower(), minutes(), moves(), setup_code(), position());
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
        char color_follower_;
        int minutes_;
        int moves_;
        SetupCode setup_code_;
        std::string position_;
};

}       // namespace dxp
}       // namespace dctl
