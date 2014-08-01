#pragma once
#include <iomanip>                      // setw
#include <sstream>                      // stringstream
#include <string>                       // string, stoi
#include <dctl/dxp/message.hpp>         // Message
#include <dctl/factory/creatable.hpp>   // make_creatable

namespace dctl {
namespace dxp {

/*

        The format and semantics of GameEnd are defined at:
        http://www.mesander.nl/damexchange/egameend.htm

*/

class GameEnd final
:
        // Curiously Recurring Template Pattern (CRTP)
        public factory::make_creatable<Message, GameEnd, 'E'>
{
public:
        enum Reason: int { forfeit = 0, resign = 1, claim_draw = 2, claim_win = 3 };
        enum StopCode: int { stop_game = 0, stop_always = 1 };

        // constructors

        explicit GameEnd(std::string const& message)
        :
                reason_{static_cast<Reason>(std::stoi(message.substr(0, 1).c_str()))},
                stop_code_{static_cast<StopCode>(std::stoi(message.substr(1, 1).c_str()))}
        {}

        // queries

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
        // virtual implementation

        std::string do_header() const override
        {
                return identifier();
        }

        std::string do_body() const override
        {
                return body(reason(), stop_code());
        }

        static std::string body(Reason r, StopCode s)
        {
                std::stringstream sstr;
                sstr << std::setw(1) << static_cast<int>(r);
                sstr << std::setw(1) << static_cast<int>(s);
                return sstr.str();
        }

        // representation

        Reason reason_;
        StopCode stop_code_;
};

}       // namespace dxp
}       // namespace dctl
