#pragma once
#include <iomanip>                      // std::setfill, std::setw
#include <memory>                       // std::unique_ptr
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/lexical_cast.hpp>       // boost::lexical_cast
#include "MessageInterface.hpp"

namespace dctl {
namespace dxp {

/*

        The BackRequest class is a <ConcreteProduct> in a <Factory Method>
        design pattern, with the Parser class as the <ConcreteCreator> and 
        the MessageInterface class as the <Product>.

        The BackRequest class registers itself with the factory.

        The format and semantics of BackRequest are explained at:
        http://www.mesander.nl/damexchange/ebackreq.htm

*/
        
class BackRequest
: 
        public MessageInterface
{
public:
        // views
        int move_number() const
        {
                return move_number_;
        }

        char side_to_move() const
        {
                return side_to_move_;
        }

        static std::string generate(int m, char c)
        {
                return HEADER_ + body(m, c);
        }

private:
        // factory creation
        static std::unique_ptr<MessageInterface> create(const std::string& message)
        {
                return std::unique_ptr<BackRequest>(new BackRequest(message));
        }

        // private constructor
        explicit BackRequest(const std::string& message)
        :
                move_number_(boost::lexical_cast<int>(message.substr(0, 3).c_str())),
                side_to_move_(*(message.substr(3, 1)).begin())
        {
        }

        // implementation
        virtual std::string header() const
        {
                return HEADER_;
        }

        virtual std::string body() const
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

        static const std::string HEADER_;
        static const bool REGISTERED_;

        // representation
        int move_number_;
        char side_to_move_;
};

}       // namespace dxp
}       // namespace dctl
