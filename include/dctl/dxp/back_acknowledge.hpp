#pragma once
#include <dctl/dxp/message.hpp>         // Message
#include <dctl/factory/creatable.hpp>   // make_creatable
#include <iomanip>                      // setfill, setw
#include <sstream>                      // stringstream
#include <string>                       // stoi, string

namespace dctl {
namespace dxp {

/*

        The format and semantics of BackAcknowledge_ are explained at:
        http://www.mesander.nl/damexchange/ebackacc.htm

*/

class BackAcknowledge final
:
        // Curiously Recurring Template Pattern (CRTP)
        public factory::make_creatable<Message, BackAcknowledge, 'K'>
{
public:
        enum AcceptanceCode: int { accept = 0, not_supported = 1, decline = 2 };

        // constructors

        explicit BackAcknowledge(std::string const& message)
        :
                acceptance_code_ { static_cast<AcceptanceCode>(std::stoi(message.substr(0, 1).c_str())) }
        {}

        // observers

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

        std::string do_header() const override
        {
                return identifier();
        }

        std::string do_body() const override
        {
                return body(acceptance_code());
        }

        static std::string body(AcceptanceCode a)
        {
                std::stringstream sstr;
                sstr << std::setw(1) << static_cast<int>(a);
                return sstr.str();
        }

        // representation

        AcceptanceCode acceptance_code_;
};

}       // namespace dxp
}       // namespace dctl
