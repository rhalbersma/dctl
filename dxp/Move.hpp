#pragma once
#include <iomanip>                      // std::setfill, std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <vector>                       // std::vector
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"
#include "Parser.hpp"
#include "Protocol.hpp"

namespace dctl {
namespace dxp {

/*

        The Move class is a <ConcreteProduct> in a <Factory Method>
        design pattern, with the Parser class as the <ConcreteCreator> 
        and the MessageInterface class as the <Product>.

        The Move class registers itself with the factory.

        The format and semantics of Move are defined at:
        http://www.mesander.nl/damexchange/emove.htm

*/
        
template
<
        typename Protocol = protocol_v1,
        template<typename> class Interface = MessageInterface, 
        typename Factory = Parser
>
class Move_
: 
        public Interface<Protocol>
{
public:
        // views
        int seconds() const
        {
                return seconds_;
        }

        int from_sq() const
        {
                return from_sq_;
        }

        int dest_sq() const
        {
                return dest_sq_;
        }

        int num_captured() const
        {
                return num_captured_;
        }

        const std::vector<int>& captured_pieces() const
        {
                return captured_pieces_;
        }

        static std::string header()
        {
                return std::string(1, HEADER_);
        }

        static std::string generate(int s, int f, int d, int n, const std::vector<int>& c)
        {
                return header() + body(s, f, d, n, c);
        }

private:
        // factory creation
        typedef Interface<Protocol> InterfaceVersion;
        static std::unique_ptr<InterfaceVersion> create(const std::string& message)
        {
                return std::unique_ptr<Move_>(new Move_(message));
        }

        // private constructor
        explicit Move_(const std::string& message)
        :
                seconds_(boost::lexical_cast<int>(message.substr(0, 4).c_str())),
                from_sq_(boost::lexical_cast<int>(message.substr(4, 2).c_str())),
                dest_sq_(boost::lexical_cast<int>(message.substr(6, 2).c_str())),
                num_captured_(boost::lexical_cast<int>(message.substr(8, 2).c_str()))
        {
                for (auto i = 0; i < num_captured(); ++i)
                        captured_pieces_.push_back(boost::lexical_cast<int>(message.substr(10 + 2 * i, 2).c_str()));
        }

        // implementation
        virtual std::string do_header() const
        {
                return header();
        }

        virtual std::string do_body() const
        {
                return body(seconds(), from_sq(), dest_sq(), num_captured(), captured_pieces());
        }

        static std::string body(int s, int f, int d, int n, const std::vector<int>& c)
        { 
                std::stringstream sstr;
                sstr << std::setw( 4) << std::setfill('0') << s;
                sstr << std::setw( 2) << std::setfill('0') << f;
                sstr << std::setw( 2) << std::setfill('0') << d;
                sstr << std::setw( 2) << std::setfill('0') << n;
                std::for_each(c.begin(), c.end(), [&sstr](int x) { 
                        sstr << std::setw(2) << std::setfill('0') << x; 
                });
                return sstr.str();
        }

        BOOST_STATIC_CONSTANT(auto, HEADER_ = 'M');
        static bool registered_;

        // representation
        int seconds_;
        int from_sq_;
        int dest_sq_;
        int num_captured_;
        std::vector<int> captured_pieces_;       
};

template<typename Protocol, template<typename> class Interface, typename Factory>
bool Move_<Protocol, Interface, Factory>::registered_ = 
        Factory::register_message(header(), create)
;

template class Move_<>;
typedef Move_<> Move;

}       // namespace dxp
}       // namespace dctl
