#pragma once
#include <iomanip>                      // std::setfill, std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"
#include "Mixin.hpp"

namespace dctl {
namespace dxp {

/*

        The BackAcknowledge_ class is a <ConcreteProduct> in a <Factory Method> 
        design pattern, with the Parser class as the <ConcreteCreator> and the 
        MessageInterface class as the <Product>.

        The BackAcknowledge_ class MUST be registered with a factory.

        The format and semantics of BackAcknowledge_ are explained at:
        http://www.mesander.nl/damexchange/ebackacc.htm

*/
        
class BackAcknowledge
: 
        public MessageInterface
{
public:
        // typedefs
        enum AcceptanceCode { accept = 0, not_supported = 1, decline = 2 };

        // views
        AcceptanceCode acceptance_code() const
        {
                return acceptance_code_;
        }

        static std::string generate(AcceptanceCode a)
        {
                return header() + body(a);
        }

        // factory creation (NOTE: makes constructor private)
        MIXIN_FACTORY_CREATION(BackAcknowledge)

        explicit BackAcknowledge(const std::string& message)
        :
                acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<int>(message.substr(0, 1).c_str())))
        {
        }

private:
        // implementation
        MIXIN_MESSAGE_HEADER('K')

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
