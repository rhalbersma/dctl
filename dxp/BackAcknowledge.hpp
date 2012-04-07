#pragma once
#include <iomanip>                      // setfill, setw
#include <sstream>                      // stringstream
#include <string>                       // string
#include <boost/lexical_cast.hpp>       // lexical_cast
#include "MessageInterface.hpp"
#include "../factory/mixin.hpp"

namespace dctl {
namespace dxp {

/*

        The format and semantics of BackAcknowledge_ are explained at:
        http://www.mesander.nl/damexchange/ebackacc.htm

*/

class BackAcknowledge
:
        // Curiously Recurring Template Pattern (CRTP)
        public mixin::IdentifierCreate<'K', BackAcknowledge, MessageInterface>
{
public:
        // typedefs
        enum AcceptanceCode { accept = 0, not_supported = 1, decline = 2 };

        explicit BackAcknowledge(const std::string& message)
        :
                acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<int>(message.substr(0, 1).c_str())))
        {
        }

        // views
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
        // implementation
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
