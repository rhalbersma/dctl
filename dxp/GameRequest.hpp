#pragma once
#include <iomanip>                      // std::setfill, std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"
#include "Parser.hpp"
#include "Version.hpp"

namespace dctl {
namespace dxp {

/*

        The GameRequest class is a <ConcreteProduct> in a <Factory Method>
        design pattern, with the Parser class as the <ConcreteCreator> and 
        the MessageInterface class as the <Product>.

        The GameRequest class registers itself with the factory.

        The format and semantics of GameRequest are defined at:
        http://www.mesander.nl/damexchange/egamereq.htm

*/

template
<
        typename Protocol = protocol,
        template<typename> class Interface = MessageInterface, 
        typename Factory = Parser
>
class GameRequest_
: 
        public Interface<Protocol>
{
public:
        // views
        const std::string& name_initiator() const
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

        bool setup() const
        {
                return setup_;
        }

        const std::string& position() const
        {
                return position_;
        }

        static std::string header()
        {
                return std::string(1, HEADER_);
        }

        static std::string generate(const std::string& n, char c, int min, int mov, bool s, const std::string& p)
        {
                return header() + body(n, c, min, mov, s, p);
        }

private:
        // factory creation
        typedef Interface<Protocol> InterfaceVersion;
        static std::unique_ptr<InterfaceVersion> create(const std::string& message)
        {
                return std::unique_ptr<GameRequest_>(new GameRequest_(message));
        }

        // private constructor
        explicit GameRequest_(const std::string& message)
        :
                name_initiator_(message.substr(2, 32)),
                color_follower_(*(message.substr(34, 1)).begin()),
                minutes_(boost::lexical_cast<int>(message.substr(35, 3).c_str())),
                moves_(boost::lexical_cast<int>(message.substr(38, 3).c_str())),
                setup_(read_setup(*(message.substr(41, 1)).begin()))
        {
                if (setup())
                        position_ = message.substr(42);
        }

        // implementation
        virtual std::string do_header() const
        {
                return header();
        }

        virtual std::string do_body() const
        {
                return body(name_initiator(), color_follower(), minutes(), moves(), setup(), position());
        }

        static std::string body(const std::string& n, char c, int min, int mov, bool s, const std::string& p)
        {
                std::stringstream sstr;
                sstr << std::setw( 2) << std::setfill('0') << PROTOCOL_version;
                sstr << std::setw(32) << n << std::setfill(' ');
                sstr << std::setw( 1) << c;
                sstr << std::setw( 3) << std::setfill('0') << min;
                sstr << std::setw( 3) << std::setfill('0') << mov;
                sstr << std::setw( 1) << write_setup(s);
                if (s)
                        sstr << std::setw(51) << p;
                return sstr.str();
        }

        static bool read_setup(char c)
        {
                switch(c) {                
                case INITIAL: 
                        return false;
                case SPECIAL: 
                        return true;
                default:
                        BOOST_ASSERT(false);
                        return false;
                }
        }

        static char write_setup(bool b)
        {
                return (!b)? INITIAL : SPECIAL;
        }

        BOOST_STATIC_CONSTANT(auto, PROTOCOL_version = 1);
        BOOST_STATIC_CONSTANT(auto, INITIAL = 'A');
        BOOST_STATIC_CONSTANT(auto, SPECIAL = 'B');        
        BOOST_STATIC_CONSTANT(auto, HEADER_ = 'R');
        static bool registered_;        
        
        // representation
        std::string name_initiator_;
        char color_follower_;
        int minutes_;
        int moves_;
        bool setup_;
        std::string position_;
};

// factory registration
template<typename Protocol, template<typename> class Interface, typename Factory>
bool GameRequest_<Protocol, Interface, Factory>::registered_ = 
        Factory::register_message(header(), create)
;

// explicit instantation
template class GameRequest_<>;
typedef GameRequest_<> GameRequest;

}       // namespace dxp
}       // namespace dctl
