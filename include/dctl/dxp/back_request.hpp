#pragma once
#include <dctl/dxp/message.hpp>         // Message
#include <dctl/factory/creatable.hpp>   // make_creatable
#include <iomanip>                      // setfill, setw
#include <sstream>                      // stringstream
#include <string>                       // string, stoi

namespace dctl {
namespace dxp {

/*

        The format and semantics of BackRequest are explained at:
        http://www.mesander.nl/damexchange/ebackreq.htm

*/

class BackRequest final
:
        // Curiously Recurring Template Pattern (CRTP)
        public factory::make_creatable<Message, BackRequest, 'B'>
{
public:
        // constructors

        explicit BackRequest(std::string const& message)
        :
                move_number_ { std::stoi(message.substr(0, 3).c_str()) },
                side_to_move_ { *(std::begin(message.substr(3, 1))) }
        {}

        // queries

        int move_number() const
        {
                return move_number_;
        }

        char side_to_move() const
        {
                return side_to_move_;
        }

        // output

        static std::string str(int m, char c)
        {
                return identifier() + body(m, c);
        }

private:
        // virtual implementation

        std::string do_header() const override
        {
                return identifier();
        }

        std::string do_body() const override
        {
                return body(move_number(), side_to_move());
        }

        static std::string body(int m, char c)
        {
                std::stringstream sstr;
                sstr << std::setw( 3) << std::setfill('0') << m;
                sstr << std::setw( 1) << c;
                return sstr.str();
        }

        // representation

        int move_number_;
        char side_to_move_;
};

}       // namespace dxp
}       // namespace dctl
