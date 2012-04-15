#pragma once
#include <iomanip>                      // setw
#include <sstream>                      // stringstream
#include <string>                       // string
#include <boost/lexical_cast.hpp>       // lexical_cast
#include "MessageInterface.hpp"
#include "../factory/mixin.hpp"

namespace dctl {
namespace dxp {

/*

        The format and semantics of GameEnd are defined at:
        http://www.mesander.nl/damexchange/egameend.htm

*/

class GameEnd
:
        // Curiously Recurring Template Pattern (CRTP)
        public mixin::FactoryCreate<'E', GameEnd, MessageInterface>
{
public:
        // typedefs
        enum Reason { forfeit = 0, resign = 1, claim_draw = 2, claim_win = 3 };
        enum StopCode { stop_game = 0, stop_always = 1 };

        explicit GameEnd(const std::string& message)
        :
                reason_(static_cast<Reason>(boost::lexical_cast<int>(message.substr(0, 1).c_str()))),
                stop_code_(static_cast<StopCode>(boost::lexical_cast<int>(message.substr(1, 1).c_str())))
        {
        }

        // views
        Reason reason() const
        {
                return reason_;
        }

        StopCode stop_code() const
        {
                return stop_code_;
        }

        // output
        static std::string str(Reason r, StopCode s)
        {
                return identifier() + body(r, s);
        }

private:
        // implementation
        virtual std::string do_header() const
        {
                return identifier();
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

        // representation
        Reason reason_;
        StopCode stop_code_;
};

}       // namespace dxp
}       // namespace dctl
