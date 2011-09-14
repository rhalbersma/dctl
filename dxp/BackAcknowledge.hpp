#pragma once
#include <iomanip>                      // std::setfill, std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"
#include "Parser.hpp"
#include "DXP.h"

namespace dctl {
namespace dxp {

/*

        The BackAcknowledge class is a <ConcreteProduct> in a <Factory Method> 
        design pattern, with the Parser class as the <ConcreteCreator> and the 
        MessageInterface class as the <Product>.

        The BackAcknowledge class registers itself with the factory.

        The format and semantics of BackAcknowledge are explained at:
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
                return HEADER_ + body(a);
        }

private:
        // factory creation
        static std::unique_ptr<MessageInterface> create(const std::string& message)
        {
                return std::unique_ptr<BackAcknowledge>(new BackAcknowledge(message));
        }

        // private constructor
        explicit BackAcknowledge(const std::string& message)
        :
                acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<int>(message.substr(0, 1).c_str())))
        {
        }

        // implementation
        virtual std::string header() const
        {
                return HEADER_;
        }

        virtual std::string body() const
        {
                return body(acceptance_code());
        }

        static std::string body(AcceptanceCode a)
        {
                std::stringstream sstr;
                sstr << std::setw(1) << a;
                return sstr.str();
        }

        static const std::string HEADER_;
        static const bool REGISTERED_;

        // representation
        AcceptanceCode acceptance_code_;
};

}       // namespace dxp
}       // namespace dctl
