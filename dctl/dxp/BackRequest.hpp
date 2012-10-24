#pragma once
#include <iomanip>                      // setfill, setw
#include <sstream>                      // stringstream
#include <string>                       // string
#include <boost/lexical_cast.hpp>       // lexical_cast
#include <dctl/dxp/IMessage.hpp>             // IMessage
#include <dctl/factory/mixin.hpp>            // FactoryCreate

namespace dctl {
namespace dxp {

/*

        The format and semantics of BackRequest are explained at:
        http://www.mesander.nl/damexchange/ebackreq.htm

*/

class BackRequest
:
        // Curiously Recurring Template Pattern (CRTP)
        public mixin::FactoryCreate<'B', BackRequest, IMessage>
{
public:
        // structors

        explicit BackRequest(std::string const& message)
        :
                move_number_(boost::lexical_cast<int>(message.substr(0, 3).c_str())),
                side_to_move_(*(std::begin(message.substr(3, 1))))
        {
        }

        // queries

        int move_number() const
        {
                return (move_number_);
        }

        char side_to_move() const
        {
                return (side_to_move_);
        }

        // output

        static std::string str(int m, char c)
        {
                return (identifier() + body(m, c));
        }

private:
        // virtual implementation

        virtual std::string do_header() const
        {
                return (identifier());
        }

        virtual std::string do_body() const
        {
                return (body(move_number(), side_to_move()));
        }

        static std::string body(int m, char c)
        {
                std::stringstream sstr;
                sstr << std::setw( 3) << std::setfill('0') << m;
                sstr << std::setw( 1) << c;
                return (sstr.str());
        }

        // representation

        int move_number_;
        char side_to_move_;
};

}       // namespace dxp
}       // namespace dctl
