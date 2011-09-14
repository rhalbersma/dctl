#pragma once
#include <string>                       // std::string

namespace dctl {
namespace dxp {

/*

        The MessageInterface class is the <Product> in a <Factory Method>
        design pattern, with object instances of its derived classes as  
        <ConcreteProduct>, and with the Parser class as the <ConcreteCreator>.

        All derived message classes MUST register themselves with the factory.

        The format and semantics of MessageInterface are defined at:
        http://www.mesander.nl/damexchange/edxplg2.htm

*/

class MessageInterface
{
public:
        // virtual destructor
        virtual ~MessageInterface() { /* no-op */ }

        // non-virtual interface
        std::string str() const 
        { 
                return header() + body(); 
        }

private:
        // pure virtual implementation
        virtual std::string header() const = 0;
        virtual std::string body() const = 0;
};

}       // namespace dxp
}       // namespace dctl
