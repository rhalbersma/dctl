#pragma once
#include <iomanip>                      // std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"
#include "Mixin.hpp"

namespace dctl {
namespace dxp {

/*

        The GameEnd class is a <ConcreteProduct> in a <Factory Method>
        design pattern, with the Parser class as the <ConcreteCreator> 
        and the MessageInterface class as the <Product>.

        The GameEnd class MUST be registered with a factory.

        The format and semantics of GameEnd are defined at:
        http://www.mesander.nl/damexchange/egameend.htm

*/

class GameEnd
: 
        public MessageInterface
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

        static std::string generate(Reason r, StopCode s)
        {
                return header() + body(r, s);
        }

        // factory creation (NOTE: makes constructor private)
        MIXIN_FACTORY_CREATION(GameEnd)

        explicit GameEnd(const std::string& message)
        :
                reason_(static_cast<Reason>(boost::lexical_cast<int>(message.substr(0, 1).c_str()))),
                stop_code_(static_cast<StopCode>(boost::lexical_cast<int>(message.substr(1, 1).c_str())))
        {
        }

private:
        // implementation
        MIXIN_MESSAGE_HEADER('E')

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

        // representation
        Reason reason_;
        StopCode stop_code_;
};

}       // namespace dxp
}       // namespace dctl