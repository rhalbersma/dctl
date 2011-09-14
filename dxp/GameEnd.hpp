#pragma once
#include <iomanip>                      // std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"

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
                return HEADER_ + body(r, s);
        }

private:
        // factory creation
        static std::unique_ptr<MessageInterface> create(const std::string& message)
        {
                return std::unique_ptr<GameEnd>(new GameEnd(message));
        }

        // private constructor
        explicit GameEnd(const std::string& message)
        :
                reason_(static_cast<Reason>(boost::lexical_cast<int>(message.substr(0, 1).c_str()))),
                stop_code_(static_cast<StopCode>(boost::lexical_cast<int>(message.substr(1, 1).c_str())))
        {
        }

        // implementation
        virtual std::string header() const
        {
                return HEADER_;
        }

        virtual std::string body() const
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

        static const std::string HEADER_;
        static const bool REGISTERED_;

        // representation
        Reason reason_;
        StopCode stop_code_;
};

}       // namespace dxp
}       // namespace dctl
