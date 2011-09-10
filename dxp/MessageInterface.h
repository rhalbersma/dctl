#pragma once
#include <string>       // std::string

namespace dctl {
namespace dxp {

/*

The MessageInterface class is the <Product> in a <Factory Method Pattern> with 
its derived classes derived as instances of a <ConcreteProduct>, and with the
Parser class as the <ConcreteCreator>.

All derived message classes MUST register themselves with the factory.

*/

class MessageInterface
{
public:
        // virtual destructor
        virtual ~MessageInterface() {};

        // non-virtual interface
        std::string str() const;

private:
        // pure virtual implementation
        virtual std::string header() const = 0;
        virtual std::string body() const = 0;
};

}       // namespace dxp
}       // namespace dctl
