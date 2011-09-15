#pragma once
#include <iomanip>                      // std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"
#include "Parser.hpp"
#include "Protocol.hpp"

namespace dctl {
namespace dxp {

/*

        The GameEnd class is a <ConcreteProduct> in a <Factory Method>
        design pattern, with the Parser class as the <ConcreteCreator> 
        and the MessageInterface class as the <Product>.

        The GameEnd class registers itself with the factory.

        The format and semantics of GameEnd are defined at:
        http://www.mesander.nl/damexchange/egameend.htm

*/

template
<
        typename Protocol = protocol_v1,
        template<typename> class Interface = MessageInterface, 
        typename Factory = Parser
>
class GameEnd_
: 
        public Interface<Protocol>
{
public:
        // typedefs
        enum Reason { forfeit = 0, resign = 1, claim_draw = 2, claim_win = 3 }; 
        enum StopCode { stop_game = 0, stop_always = 1 };

        // views
        Reason reason() const
        {
                return reason_;
        }

        StopCode stop_code() const
        {
                return stop_code_;
        }

        static std::string header()
        {
                return std::string(1, HEADER_);
        }

        static std::string generate(Reason r, StopCode s)
        {
                return header() + body(r, s);
        }

private:
        // factory creation
        typedef Interface<Protocol> InterfaceVersion;
        static std::unique_ptr<InterfaceVersion> create(const std::string& message)
        {
                return std::unique_ptr<GameEnd_>(new GameEnd_(message));
        }

        // private constructor
        explicit GameEnd_(const std::string& message)
        :
                reason_(static_cast<Reason>(boost::lexical_cast<int>(message.substr(0, 1).c_str()))),
                stop_code_(static_cast<StopCode>(boost::lexical_cast<int>(message.substr(1, 1).c_str())))
        {
        }

        // implementation
        virtual std::string do_header() const
        {
                return header();
        }

        virtual std::string do_body() const
        {
                return body(reason(), stop_code());
        }

        static std::string body(Reason r, StopCode s)
        {
                std::stringstream sstr;
                sstr << std::setw( 1) << r;
                sstr << std::setw( 1) << s;
                return sstr.str();
        }

        BOOST_STATIC_CONSTANT(auto, HEADER_ = 'E');
        static bool registered_;

        // representation
        Reason reason_;
        StopCode stop_code_;
};

// factory registration
template<typename Protocol, template<typename> class Interface, typename Factory>
bool GameEnd_<Protocol, Interface, Factory>::registered_ = 
        Factory::register_message(header(), create)
;

// explicit instantation
template class GameEnd_<>;
typedef GameEnd_<> GameEnd;

}       // namespace dxp
}       // namespace dctl
