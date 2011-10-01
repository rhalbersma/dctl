#pragma once
#include <algorithm>                    // std::for_each
#include <iomanip>                      // std::setfill, std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <vector>                       // std::vector
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"
#include "Mixin.hpp"

namespace dctl {
namespace dxp {

/*

        The Move class is a <ConcreteProduct> in a <Factory Method>
        design pattern, with the Parser class as the <ConcreteCreator> 
        and the MessageInterface class as the <Product>.

        The Move class MUST be registered with a factory.

        The format and semantics of Move are defined at:
        http://www.mesander.nl/damexchange/emove.htm

*/
        
class Move
: 
        public MessageInterface
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

        static std::string generate(int s, int f, int d, int n, const std::vector<int>& c)
        {
                return header() + body(s, f, d, n, c);
        }

        // factory creation (NOTE: makes constructor private)
        MIXIN_FACTORY_CREATION(Move)

        explicit Move(const std::string& message)
        :
                seconds_(boost::lexical_cast<int>(message.substr(0, 4).c_str())),
                from_sq_(boost::lexical_cast<int>(message.substr(4, 2).c_str())),
                dest_sq_(boost::lexical_cast<int>(message.substr(6, 2).c_str())),
                num_captured_(boost::lexical_cast<int>(message.substr(8, 2).c_str()))
        {
                for (auto i = 0; i < num_captured(); ++i)
                        captured_pieces_.push_back(boost::lexical_cast<int>(message.substr(10 + 2 * i, 2).c_str()));
        }

private:
        // implementation
        MIXIN_MESSAGE_HEADER('M')

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

        // representation
        int seconds_;
        int from_sq_;
        int dest_sq_;
        int num_captured_;
        std::vector<int> captured_pieces_;       
};

}       // namespace dxp
}       // namespace dctl
