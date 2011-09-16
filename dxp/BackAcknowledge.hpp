#pragma once
#include <iomanip>                      // std::setfill, std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"
#include "Parser.hpp"
#include "Version.hpp"

namespace dctl {
namespace dxp {

/*

        The BackAcknowledge_ class is a <ConcreteProduct> in a <Factory Method> 
        design pattern, with the Parser class as the <ConcreteCreator> and the 
        MessageInterface class as the <Product>.

        The BackAcknowledge_ class registers itself with the factory.

        The format and semantics of BackAcknowledge_ are explained at:
        http://www.mesander.nl/damexchange/ebackacc.htm

*/
        
template
<
        typename Protocol = protocol,
        template<typename> class Interface = MessageInterface, 
        typename class Factory = Parser
>
class BackAcknowledge_
: 
        public Interface<Protocol>
{
public:
        // typedefs
        enum AcceptanceCode { accept = 0, not_supported = 1, decline = 2 };

        // views
        AcceptanceCode acceptance_code() const
        {
                return acceptance_code_;
        }

        static std::string header()
        {
                return std::string(1, HEADER_);
        }

        static std::string generate(AcceptanceCode a)
        {
                return header() + body(a);
        }

private:
        // factory creation
        typedef Interface<Protocol> InterfaceVersion;
        static std::unique_ptr<InterfaceVersion> create(const std::string& message)
        {
                return std::unique_ptr<BackAcknowledge_>(new BackAcknowledge_(message));
        }

        // private constructor
        explicit BackAcknowledge_(const std::string& message)
        :
                acceptance_code_(static_cast<AcceptanceCode>(boost::lexical_cast<int>(message.substr(0, 1).c_str())))
        {
        }

        // implementation
        virtual std::string do_header() const
        {
                return header();
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

        BOOST_STATIC_CONSTANT(auto, HEADER_ = 'K');
        static bool registered_;

        // representation
        AcceptanceCode acceptance_code_;
};

// factory registration
template<typename Protocol, template<typename> class Interface, typename Factory>
bool BackAcknowledge_<Protocol, Interface, Factory>::registered_ = 
        Factory::register_message(header(), create)
;

// explicit instantation
template class BackAcknowledge_<>;
typedef BackAcknowledge_<> BackAcknowledge;

}       // namespace dxp
}       // namespace dctl
