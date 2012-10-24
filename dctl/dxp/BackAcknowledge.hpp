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

        The format and semantics of BackAcknowledge_ are explained at:
        http://www.mesander.nl/damexchange/ebackacc.htm

*/

class BackAcknowledge
:
        // Curiously Recurring Template Pattern (CRTP)
        public mixin::FactoryCreate<'K', BackAcknowledge, IMessage>
{
public:
        // typedefs

        enum AcceptanceCode { accept = 0, not_supported = 1, decline = 2 };

        // structors

        explicit BackAcknowledge(std::string const& message)
        :
                acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<int>(message.substr(0, 1).c_str())))
        {
        }

        // queries

        AcceptanceCode acceptance_code() const
        {
                return acceptance_code_;
        }

        // output
        static std::string str(AcceptanceCode a)
        {
                return identifier() + body(a);
        }

private:
        // virtual implementation

        virtual std::string do_header() const
        {
                return identifier();
        }

        virtual std::string do_body() const
        {
                return body(acceptance_code());
        }

        static std::string body(AcceptanceCode a)
        {
                std::stringstream sstr;
                sstr << std::setw(1) << a;
                return sstr.str();
        }

        // representation

        AcceptanceCode acceptance_code_;
};

}       // namespace dxp
}       // namespace dctl
